/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "App.h"
#include "Config.h"
#include "Context.h"
#include "Logging.h"
#include "controller/DefaultQuitHandler.h"
#include "controller/QuitEvent.h"
#include "controller/RefreshEvent.h"
#include "controller/RefreshEvent.h"
#include "controller/SubmitEvent.h"
#include "mapping/TableMapping.h"
#include "view/AbstractView.h"
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>
#include <memory>
#include <time.h>

namespace GtkForms {
using namespace Container;
using namespace Wrapper;
using namespace Core;

static src::logger_mt &lg = logger::get ();

gboolean guiThread (gpointer user_data);

/**
 *
 */
struct App::Impl {

        struct ControllerOperation {
                /// true : open, false : close.
                enum { OPEN, CLOSE } type;
                AbstractController *requestor;
                std::string controllerName;
        };

        void controllerRemoval (AbstractController *controller, bool removeFromParent);
        void controllerOpen (std::string const &controllerName, AbstractController *requestor, App *app);
        AbstractView *loadView (std::string const &viewAndSlot, AbstractController *controller);
        static IMapping *getDefaultMapping (ViewElementDTO *vd);

        typedef std::vector<ControllerOperation> ControllerOperationVector;
        ControllerOperationVector controllerOperations;
        AbstractController *rootController = nullptr;

        EventStack events;

        Ptr<BeanFactoryContainer> container;
        Context context;
        Config *config = nullptr;
        bool controllersIdling = true;

        unsigned int getCurrentMs () const;
        unsigned int lastMs = 0;
        bool quitRequested = false;
        DefaultQuitHandler defaultQuitHandler;
        bool beanWrapperInitialized = false;
        Wrapper::BeanWrapper *beanWrapper = 0;

        //        TODO w forrest jest potrzbene cofanie
        //        typedef std::stack<std::string> UnitNameStack;
        //        UnitNameStack previousUnits;
};

/*--------------------------------------------------------------------------*/

// App::App (std::string const &configurationFile, std::string const &initialControllerName) { init (configurationFile, initialControllerName); }

/*---------------------------------------------------------------------------*/

void App::init (std::string const &configurationFile, std::string const &initialControllerName)
{
        impl = new Impl;
        initContainer (configurationFile);
        g_idle_add (guiThread, static_cast<gpointer> (this));
        getContext ().setToSessionScope ("app", Core::Variant (this));
        impl->controllerOperations.push_back (Impl::ControllerOperation{ Impl::ControllerOperation::OPEN, nullptr, initialControllerName });
}

/*--------------------------------------------------------------------------*/

App::~App () { delete impl; }

/*--------------------------------------------------------------------------*/

void App::run ()
{
        unsigned int currentMs = impl->getCurrentMs ();
        if (impl->lastMs + impl->config->loopDelayMs <= currentMs) {
                impl->lastMs = currentMs;
                manageControllers ();

                if (impl->controllersIdling && impl->rootController) {
                        impl->rootController->performIdle (currentMs);
                }
        }
        else {
                usleep (((impl->lastMs + impl->config->loopDelayMs) - currentMs) * 1000);
        }
}

/*****************************************************************************/

// TODO to spokojnie mogłaby być metoda kontrolera.
void App::Impl::controllerRemoval (AbstractController *controller, bool removeFromParent)
{
        for (AbstractController *child : controller->getChildren ()) {
                controllerRemoval (child, false);
        }

        AbstractView *view = controller->getView ();
        // TODO jeśli controller jest singletonem, to tylkol hide.? na pewno? Kontorlery chyba tworzą te widoki od nowa.
        if (view) {
                view->setControllerToUi (nullptr);
                view->destroyUi ();
        }

        controller->onStop ();

        // Clear some fields.
        controller->setApp (nullptr);
        controller->setView (nullptr);
        controller->clearControllerScope ();
        controller->getChildren ().clear ();

        if (!removeFromParent) {
                return;
        }

        // Usuń zadany kontroler z aktualnej struktury drzewiastej
        if (!controller->getParent ()) {
                rootController = nullptr;
        }
        else {
                AbstractController *parent = controller->getParent ();
                ControllerVector &children = parent->getChildren ();
                auto i = std::remove (children.begin (), children.end (), controller);
                children.erase (i, children.end ());
        }
}

/*****************************************************************************/

// TODO to spokojnie mogłaby być metoda kontrolera.
void App::Impl::controllerOpen (std::string const &controllerName, AbstractController *requestor, App *app)
{
        AbstractController *controller = ocast<AbstractController *> (container->getBean (controllerName));

        if (!controller) {
                throw Core::Exception ("Can't find controller with name : [" + controllerName + "] in container.");
        }

        controller->setApp (app);
        controller->clearControllerScope ();

        if (requestor) {
                requestor->getChildren ().push_back (controller);
        }
        else {
                rootController = controller;
        }

        controller->setParent (requestor);

        std::string viewName = controller->onStart ();
        AbstractView *view = AbstractView::loadView (viewName, controller, container);
        controller->setView (view);
        view->runDecorators (IPageDecorator::POST_SHOW, &context);

        Core::StringVector alsoOpenList;
        if (!controller->alsoOpen.empty ()) {
                boost::split (alsoOpenList, controller->alsoOpen, boost::is_any_of (", "), boost::token_compress_on);
        }

        for (std::string const &name : alsoOpenList) {
                controllerOpen (name, controller, app);
        }
}

/*****************************************************************************/

void App::manageControllers ()
{
        /*---------------------------------------------------------------------------*/

        // Kontrolery do zamknięcia
        for (App::Impl::ControllerOperation const &operation : impl->controllerOperations) {
                if (operation.type == App::Impl::ControllerOperation::CLOSE) {

                        // Find the controller that is to be closed.
                        AbstractController *controller = nullptr;

                        if (!operation.controllerName.empty ()) {
                                controller = impl->rootController->findByName (operation.controllerName);
                        }
                        else {
                                controller = operation.requestor;
                        }

                        impl->controllerRemoval (controller, true);
                }
        }

        // Kontrolery do otworzenia
        for (App::Impl::ControllerOperation const &operation : impl->controllerOperations) {
                if (operation.type == App::Impl::ControllerOperation::OPEN) {
                        impl->controllerOpen (operation.controllerName, operation.requestor, this);
                }
        }

        impl->controllerOperations.clear ();

        while (!impl->events.empty ()) {
                std::unique_ptr<IEvent> event = std::move (impl->events.top ());
                impl->events.pop ();
                event->run (this);
        }
}

/*--------------------------------------------------------------------------*/

void App::open (AbstractController *requestor, Core::StringVector const &childControllerNames)
{
        for (std::string const &name : childControllerNames) {
                open (requestor, name);
        }
}

void App::open (AbstractController *requestor, std::string const &childControllerName)
{
        impl->controllerOperations.push_back (Impl::ControllerOperation{ Impl::ControllerOperation::OPEN, requestor, childControllerName });
}

/*--------------------------------------------------------------------------*/

void App::close (AbstractController *requestor, Core::StringVector const &controllerNames)
{
        // Close requestor children.
        for (std::string const &name : controllerNames) {
                close (requestor, name);
        }

        // Close requestor itself.
        if (controllerNames.empty ()) {
                close (requestor, "");
        }
}

void App::close (AbstractController *requestor, std::string const &controllerName)
{
        impl->controllerOperations.push_back (Impl::ControllerOperation{ Impl::ControllerOperation::CLOSE, requestor, controllerName });
}

/*--------------------------------------------------------------------------*/

unsigned int App::Impl::getCurrentMs () const
{
        struct timespec spec;
        clock_gettime (CLOCK_REALTIME, &spec);
        unsigned int ms = round (spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
        return spec.tv_sec * 1000 + ms;
}

/*--------------------------------------------------------------------------*/

/**
 * TODO wywalić gdzieś.
 */
void guiLoadTheme (const char *directory, const char *theme_name, GObject *toplevel)
{
        GtkCssProvider *css_provider;
        GError *error = NULL;
        char buf[strlen (directory) + strlen (theme_name) + 32];
        /* Gtk theme is a directory containing gtk-3.0/gtkrc file */
        snprintf (buf, sizeof (buf), "%s/%s/gtk-3.0/gtk.css", directory, theme_name);
        css_provider = gtk_css_provider_new ();
        gtk_css_provider_load_from_file (css_provider, g_file_new_for_path (buf), &error);

        if (error) {
                g_warning ("%s\n", error->message);
                return;
        }

        GdkDisplay *display = gdk_display_get_default ();
        GdkScreen *screen = gdk_display_get_default_screen (display);

        gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

        g_object_unref (css_provider);
}

/*--------------------------------------------------------------------------*/

void App::pushEvent (std::unique_ptr<IEvent> e) { impl->events.push (std::move (e)); }

/*--------------------------------------------------------------------------*/

void App::userQuitRequest ()
{
        if (impl->config->quitHandler) {
                impl->config->quitHandler->run (this);
        }
        else {
                impl->defaultQuitHandler.run (this);
        }
}

/*--------------------------------------------------------------------------*/

/*
 * Metoda start kontrolera zwraca nazwę widoku. ViewResolver otrzymuje tą nazwę i podejmuje decyzję jaki widok pokazać.
 * Możliwe, że ładuje go z pojedyczego pliku. Możliwe, że nie ładuje, bo on juz jest widoczny, mozliwe, że ładuje i skleja
 * kilka widoków w jeden na podstawie jakiejś konfiguracji. Możliwe wreszcie, ze ładuje z pliku ui i embeduje w już istniejącym.
 * Jednocześnie będzie musiał umieć skasować poprzedni. I tak dalej i tak dalej.
 *
 * view->model2View ();
 * view->show ();
 *
 * I teraz widok się pokazuje z wypełnionym formularzem (pustym, bo login był pusty).
 *
 * User wypełnia dwa pola formularza o nazwach : form.login i form.password. Klika guzik OK, który ma w clicked akcję
 * $submit(). Ta akcja jakimś cudem trafia znów do App, albo do Flow:
 */
void App::doSubmit (SubmitEvent *event)
{
        BOOST_LOG (lg) << "SubmitEvent::run. dataRange : [" << event->inputRange << "], controllerName : [" << event->controllerName << "].";

        AbstractController *controller = nullptr;

        if (event->controllerName.empty ()) {
                controller = event->controller;
        }
        else {
                controller = impl->rootController->findByName (event->controllerName);
        }

        if (!controller) {
                throw Core::Exception ("You requested submit to a controller which is not currently loaded. Controller name : [" + event->controllerName
                                       + "].");
        }

        controller->getValidationResults ().clear ();
        //        impl->context.setCurrentController (controller);

        AbstractView *view = controller->getView ();
        MappingMultiMap const &mappings = view->getMappingsByInput ();

#if 0
        view->printStructure ();
#endif

        AbstractView::WidgetMap inputMap = view->getInputs (event->inputRange);
        bool hasErrors = false;

        for (auto elem : inputMap) {
                std::string inputName = elem.first;
                MappingDTO dto;
                dto.app = this;
                dto.m2vModelObject = Core::Variant (controller->getModelAccessor ());
                dto.v2mModelObject = Core::Variant (controller->getModelAccessor ());
                dto.dataRange = event->inputRange;

                ViewElementDTO elementDTO{ G_OBJECT (elem.second) };
                dto.viewElement = &elementDTO;

                MappingMultiMap::const_iterator i;
                auto eq = mappings.equal_range (inputName);
                bool mappingFoundForInput = false;
                for (i = eq.first; i != eq.second; ++i) {
                        IMapping *mapping = i->second;
                        ValidationAndBindingResult vr = mapping->view2Model (&dto);
                        mappingFoundForInput = true;
                        controller->getValidationResults ().add (vr);

                        if (!vr.valid) {
                                BOOST_LOG (lg) << "Binding error. Model : [" << vr.model << "]";
                                hasErrors = true;
                        }

                        continue;
                }

                // Falling back to default mapping if none specifica were found.
                if (!mappingFoundForInput) {
                        IMapping *defaultMapping = Impl::getDefaultMapping (&elementDTO);
                        ValidationAndBindingResult vr = defaultMapping->view2Model (&dto, inputName, "", "");
                        controller->getValidationResults ().add (vr);

                        if (!vr.valid) {
                                BOOST_LOG (lg) << "Binding error. Model : [" << vr.model << "]";
                                hasErrors = true;
                        }
                }
        }

        /*
         * Validation.
         */
        ValidatorVector const &validators = controller->getValidators ();

        for (IValidator *validator : validators) {
                ValidationAndBindingResult vr = validator->validate (impl->context);
                controller->getValidationResults ().add (vr);

                if (!vr.valid) {
                        BOOST_LOG (lg) << "Validation error. Model : [" << vr.model << "]";
                        hasErrors = true;
                }
        }

        ValidationAndBindingResult vr = controller->validate ();
        controller->getValidationResults ().add (vr);

        if (!vr.valid) {
                BOOST_LOG (lg) << "Validation error. Model : [" << vr.model << "]";
                hasErrors = true;
        }

        impl->context.setValidationResults (&controller->getValidationResults ());

        // Fire decorators.
        view->runDecorators (IPageDecorator::PRE_SUBMIT, &impl->context);

        if (!hasErrors) {
                controller->onSubmit ();
        }
}

/*--------------------------------------------------------------------------*/

IMapping *App::Impl::getDefaultMapping (ViewElementDTO *vd)
{
        GObject *inputWidget = vd->inputWidget;
        static Mapping mapping;
        // static TableMapping tableMapping;

        if (GTK_IS_TREE_VIEW (inputWidget)) {
                // return &tableMapping;
                return nullptr; // You can not get column names from a ListStore, so there is no way to know which models to use.
        }

        return &mapping;
}

/*--------------------------------------------------------------------------*/

void App::doRefresh (RefreshEvent *event)
{
        //        impl->context.setCurrentController (event->controller);

        AbstractView *view = event->controller->getView ();
        AbstractView::WidgetMap inputMap;

        // 1. From mappings
        if (!event->modelRange.empty ()) {
                MappingMultiMap mappings = view->getMappingsByModelRange (event->modelRange);

                for (MappingMultiMap::value_type &elem : mappings) {
                        AbstractView::WidgetMap tmp = view->getInputs (elem.second->getWidget (), true);

                        if (impl->config->logMappings) {
                                BOOST_LOG (lg) << "Refreshing widget named : \033[32m[" << elem.second->getWidget () << "]\033[0m (refresh range event).";
                        }

                        assert (tmp.size () == 1);
                        GtkWidget *input = tmp.begin ()->second;

                        MappingDTO dto;
                        dto.app = this;
                        dto.m2vModelObject = Core::Variant (event->controller->getModelAccessor ());
                        dto.v2mModelObject = Core::Variant (event->controller->getModelAccessor ());
                        dto.dataRange = event->modelRange;

                        ViewElementDTO elementDTO{ G_OBJECT (input) };
                        dto.viewElement = &elementDTO;

                        elem.second->model2View (&dto);
                }
        }

        /*
         * 2. From inputs
         * If model range is empty, then we have to fall back to the default behavior, which
         * would iterate over all the inputs and try to find models coresponding to those
         * inputs.
         */
        AbstractView::WidgetMap tmp = view->getInputs (event->modelRange, true);
        std::copy (tmp.begin (), tmp.end (), std::inserter (inputMap, inputMap.end ()));

        MappingMultiMap const &mappings = view->getMappingsByInput ();

        for (auto elem : inputMap) {
                std::string inputName = elem.first;

                if (impl->config->logMappings) {
                        BOOST_LOG (lg) << "Refreshing widget named : \033[32m[" << inputName << "]\033[0m (refresh all event)";
                }

                MappingDTO dto;
                dto.app = this;
                dto.m2vModelObject = Core::Variant (event->controller->getModelAccessor ());
                dto.v2mModelObject = Core::Variant (event->controller->getModelAccessor ());
                dto.dataRange = event->modelRange;

                ViewElementDTO elementDTO{ G_OBJECT (elem.second) };
                dto.viewElement = &elementDTO;

                MappingMultiMap::const_iterator i;
                auto eq = mappings.equal_range (inputName);

                bool customMappingWasRun = false;
                for (i = eq.first; i != eq.second; ++i) {
                        IMapping *mapping = i->second;

                        if (impl->config->logMappings) {
                                BOOST_LOG (lg) << "Maping found. Input : [" << mapping->getWidget () << "], model : [" << mapping->getModel () << "]";
                        }

                        mapping->model2View (&dto);
                        customMappingWasRun = true;
                }

                // Default.
                if (!customMappingWasRun) {
                        IMapping *defaultMapping = Impl::getDefaultMapping (&elementDTO);

                        if (!defaultMapping) {
                                throw Core::Exception ("No default conversion for widget : [" + inputName + "].");
                        }

                        defaultMapping->model2View (&dto, inputName, "", "");
                }
        }

        view->refresh (&impl->context);
        view->runDecorators (IPageDecorator::POST_REFRESH, &impl->context);
}

/*--------------------------------------------------------------------------*/

Ptr<Container::BeanFactoryContainer> App::createContainer (Ptr<Container::MetaContainer> metaContainer)
{
        Ptr<Container::BeanFactoryContainer> container = ContainerFactory::create (metaContainer, true);
        // Example:
        // container->addConversion (typeid (Geometry::Point), Geometry::stringToPointVariant);
        // container->addSingleton (i->first.c_str (), i->second);
        return container;
}

/*---------------------------------------------------------------------------*/

void App::initContainer (std::string const &configFile)
{
        Ptr<MetaContainer> metaContainer = CompactMetaService::parseFile (configFile);
        impl->container = createContainer (metaContainer);
        ContainerFactory::init (impl->container.get (), metaContainer.get ());
        impl->config = vcast<Config *> (impl->container->getBean ("config"));
}

/*--------------------------------------------------------------------------*/

Context &App::getContext ()
{
        /*
         * Warning : if by any chance you are facing a SegF and debugger points you here, and :
         * - this method is called by AbstractController::set
         * - the controller is created using tiliae container
         * then it may mean, that container is trying to set some non-existent property of the
         * controller, but it fails, and falls back to the "set" method, which at this point
         * can not be used, and should not be used during controller creation.
         */
        if (!impl->beanWrapperInitialized) {
                initBeanWrapper ();
        }

        return impl->context;
}

/*--------------------------------------------------------------------------*/

k202::K202 *App::getK202 ()
{
        static Ptr<k202::K202> k202 = k202::K202::create ();
        return k202.get ();
}

/*--------------------------------------------------------------------------*/

void App::initBeanWrapper ()
{
        impl->beanWrapper = new Wrapper::BeanWrapper (true);
        //                beanWrapper->addPlugin (new GObjectWrapperPlugin);
        impl->beanWrapper->addPlugin (new Wrapper::PropertyRWBeanWrapperPlugin ());
        impl->beanWrapper->addPlugin (new Wrapper::GetPutMethodRWBeanWrapperPlugin ());
        impl->beanWrapper->addPlugin (new Wrapper::MethodPlugin (Wrapper::MethodPlugin::METHOD));

        Editor::TypeEditor *typeEditor = new Editor::TypeEditor (true);
        typeEditor->setEqType (new Editor::NoopEditor ());
        typeEditor->setNullType (new Editor::NoopEditor ());

        typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (int), new Editor::StreamEditor<std::string, int> ()));
        typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (double), new Editor::LexicalEditor<std::string, double> ()));
        typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (float), new Editor::LexicalEditor<std::string, float> ()));
        typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (char), new Editor::LexicalEditor<std::string, char> ()));
        typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (bool), new Editor::LexicalEditor<std::string, bool> ()));
        typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (unsigned int), new Editor::StreamEditor<std::string, unsigned int> ()));
        typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (unsigned char), new Editor::StreamEditor<std::string, unsigned char> ()));
        typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (long), new Editor::StreamEditor<std::string, long> ()));
        typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (unsigned long), new Editor::StreamEditor<std::string, unsigned long> ()));

        Editor::ChainEditor *chain = new Editor::ChainEditor (true);
        chain->addEditor (typeEditor);
        chain->addEditor (new Editor::NoopEditor ());

        impl->beanWrapper->setEditor (chain);
        impl->beanWrapperInitialized = true;

        impl->context.setBeanWrapper (impl->beanWrapper);
}

/*--------------------------------------------------------------------------*/

Wrapper::BeanWrapper *App::getBeanWrapper ()
{
        if (!impl->beanWrapperInitialized) {
                initBeanWrapper ();
        }

        return impl->beanWrapper;
}

/*--------------------------------------------------------------------------*/

void App::quit ()
{
        BOOST_LOG (lg) << "App::quit : quit requested.";
        impl->quitRequested = true;
}

/*--------------------------------------------------------------------------*/

gboolean guiThread (gpointer userData)
{
        App *app = static_cast<App *> (userData);
        app->run ();
        if (app->impl->quitRequested) {
                gtk_main_quit ();
                return G_SOURCE_REMOVE;
        }
        else {
                return G_SOURCE_CONTINUE;
        }
}

/*---------------------------------------------------------------------------*/

Config const *App::getConfig () const { return impl->config; }

} // namespace GtkForms
