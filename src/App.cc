/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <algorithm>
#include <memory>
#include "App.h"
#include "Unit.h"
#include "Page.h"
#include "Context.h"
#include "controller/SubmitEvent.h"
#include "controller/RefreshEvent.h"
#include "controller/QuitEvent.h"
#include "controller/IController.h"
#include "view/GtkTileManager.h"
#include <boost/regex.hpp>
#include "controller/RefreshEvent.h"
#include "view/Slot.h"
#include "Logging.h"
#include "Config.h"
#include <time.h>
#include "controller/DefaultQuitHandler.h"

namespace GtkForms {
using namespace Container;
using namespace Wrapper;
using namespace Core;

static src::logger_mt& lg = logger::get();

gboolean guiThread (gpointer user_data);

/**
 *
 */
struct App::Impl {

        Unit *getUnit (std::string );
        std::pair <IView *, Page *> getActiveViewOrThrow (std::string const &viewName);

        std::string unitToStart;
        Core::StringSet unitsToJoin;
        Core::StringSet unitsToSplit;

        Core::StringSet pagesToHide;
        Core::StringSet pagesToShow;

        EventStack events;

        // Current unit.
        Unit unit;
        // Current pages.
        PageMap pages;

        Ptr <BeanFactoryContainer> container;
        Context context {getBeanWrapper()};
        GtkTileManager tileManager;
        Config *config = nullptr;
        bool controllersIdling = true;

        static Wrapper::BeanWrapper *getBeanWrapper ();
        unsigned int getCurrentMs () const;
        unsigned int lastMs = 0;
        bool quitRequested = false;
        DefaultQuitHandler defaultQuitHandler;

        typedef std::stack <std::string> UnitNameStack;
        UnitNameStack previousUnits;
};

/*--------------------------------------------------------------------------*/

App::App (std::string const &configurationFile)
{
        impl = new Impl;
        createContainer (configurationFile);
        g_idle_add (guiThread, static_cast <gpointer> (this));
        impl->context.setToSessionScope ("app", Core::Variant (this));
        impl->context.setCurrentUnit (&impl->unit);
}

/*--------------------------------------------------------------------------*/

App::~App ()
{
        delete impl;
}

/*--------------------------------------------------------------------------*/

Core::StringSet App::manageUnits ()
{
        UnitOperationResult uoResult;
        bool mainUnitModified = false;

        if (!impl->unitToStart.empty ()) {
                IUnit *unit = getUnit (impl->unitToStart);
                uoResult += impl->unit.start (unit);
                impl->previousUnits.push (impl->unitToStart);
                BOOST_LOG (lg) << "Unit to be started : name : [" << impl->unitToStart << "], unit itself : [" << *unit << "]";
                mainUnitModified = true;
        }

        for (std::string const &unitName : impl->unitsToJoin) {
                IUnit *unit = getUnit (unitName);
                uoResult += impl->unit.join (unit);
                BOOST_LOG (lg) << "Unit to be joined : name : [" << unitName << "], unit itself : [" << *unit << "]";
                mainUnitModified = true;
        }

        for (std::string const &unitName : impl->unitsToSplit) {
                IUnit *unit = getUnit (unitName);
                uoResult += impl->unit.split (unit);
                BOOST_LOG (lg) << "Unit to be split : name : [" << unitName << "], unit itself : [" << *unit << "]";
                mainUnitModified = true;
        }

        if (mainUnitModified) {
                BOOST_LOG (lg) << "Current unit looks like this : [" << impl->unit << "]";
        }

        impl->unitToStart = "";
        impl->unitsToJoin.clear ();
        impl->unitsToSplit.clear ();

        /*
         * Now that our Unit is up to date, w can deal with form submission and data conversion
         * events:
         * - submit
         * - quit
         */
        while (!impl->events.empty ()) {
                std::unique_ptr <IEvent> event = std::move (impl->events.top ());
                impl->events.pop ();
                event->run (this);
        }

        Core::StringSet viewCommands;

        // Here we are dealing with pages and thier views to hide.
        for (ControllerMap::value_type const &entry : uoResult.removed) {
                IController *controller = entry.second;
                controller->setApp (this);
                impl->context.setCurrentController (controller);
                std::string command = controller->end ();
                controller->clearFlashScope ();
                impl->context.getSessionScope ().erase (entry.first);

                if (!command.empty ()) {
                        viewCommands.insert (command);
                }
        }

        for (ControllerMap::value_type const &entry : uoResult.added) {
                IController *controller = entry.second;
                controller->setApp (this);
                controller->clearFlashScope ();
                impl->context.setCurrentController (controller);
                std::string command = controller->start ();
                impl->context.setToSessionScope (entry.first, Core::Variant (controller));

                if (!command.empty ()) {
                        viewCommands.insert (command);
                }
        }

        return viewCommands;
}

/*--------------------------------------------------------------------------*/

void App::managePages (Core::StringSet const &viewCommands)
{
        boost::regex e1 ("\\+(.*)");
        boost::regex e2 ("\\-(^[>]*)");
        boost::regex e3 ("(.*)->(.*)");
        boost::smatch what;

        for (std::string viewCommand : viewCommands) {
                if (boost::regex_match (viewCommand, what, e1, boost::match_extra)) {
                        if (what.size () == 2) {
                                addPage (what[1]);
                        }
                }
                else if (boost::regex_match (viewCommand, what, e2, boost::match_extra)) {
                        if (what.size () == 2) {
                                removePage (what[1]);
                        }
                }
                else {
                        boost::smatch what;

                        if (boost::regex_match (viewCommand, what, e3, boost::match_extra)) {

                                if (what.size () == 3) {
                                        movePage (what[1], what[2]);
                                }
                                else if (what.size () == 2) {
                                        movePage ("", what[1]);
                                }
                        }
                        else {
                                throw Core::Exception ("Invalid view command. Use +, - or -> operations.");
                        }

                }
        }
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

void App::run ()
{
        unsigned int currentMs = impl->getCurrentMs ();
        if (impl->lastMs + impl->config->loopDelayMs <= currentMs) {
                impl->lastMs = currentMs;
                Core::StringSet viewCommands = manageUnits ();
                managePages (viewCommands);

                if (impl->controllersIdling) {
                        ControllerMap &controllers = impl->unit.getControllers ();

                        for (ControllerMap::value_type const &entry : controllers) {
                                 IController *controller = entry.second;

                                 if (controller->getLoopDelayMs () < 0) {
                                         continue;
                                 }

                                 if (controller->getLastMs () + controller->getLoopDelayMs () <= currentMs) {
                                         controller->getLastMs () = currentMs;
                                         impl->context.setCurrentController (controller);
                                         controller->onIdle ();
                                 }
                        }
                }
        }
        else {
                usleep (((impl->lastMs + impl->config->loopDelayMs) - currentMs) * 1000);
        }
}

/*--------------------------------------------------------------------------*/

/**
 * TODO wywalić gdzieś.
 */
void guiLoadTheme (const char *directory, const char *theme_name, GObject *toplevel)
{
        GtkCssProvider *css_provider;
        GError *error = NULL;
        char buf[strlen(directory) + strlen(theme_name) + 32];
        /* Gtk theme is a directory containing gtk-3.0/gtkrc file */
        snprintf(buf, sizeof(buf), "%s/%s/gtk-3.0/gtk.css", directory, theme_name);
        css_provider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(css_provider, g_file_new_for_path(buf), &error);

        if (error) {
                g_warning("%s\n", error->message);
                return;
        }

        GdkDisplay *display = gdk_display_get_default();
        GdkScreen *screen = gdk_display_get_default_screen(display);

        gtk_style_context_add_provider_for_screen(screen,
                        GTK_STYLE_PROVIDER(css_provider),
                        GTK_STYLE_PROVIDER_PRIORITY_USER);

        g_object_unref (css_provider);
}

/*--------------------------------------------------------------------------*/

void App::addPage (std::string const &pageName)
{
        BOOST_LOG (lg) << "+" << pageName;

        if (impl->pages.find (pageName) != impl->pages.end ()) {
                // TODO What should happen here? Nothing? Exception? Another instance of view?
                throw Core::Exception ("Illegal attempt to open multiple instances of page : [" + pageName + "]");
        }

        // Load from container, or return if already loaded.
        Page *page = getPage (pageName);
        impl->pages[pageName] = page;

        // Load UI file, or noop if loaded.
        page->loadUi (this);

        GtkView *mainView = page->getView ();

        if (!mainView) {
                throw Core::Exception ("view property of object Page is NULL.");
        }

        page->reparent (&impl->context);
        // TODO jakoś przenieść!
//        guiLoadTheme ("/home/iwasz/.themes/", "BioMorph", G_OBJECT (mainView));
        page->show ();
}

/*--------------------------------------------------------------------------*/

void App::removePage (std::string const &pageName)
{
        BOOST_LOG (lg) << "-" << pageName;

        if (impl->pages.find (pageName) != impl->pages.end ()) {
                throw Core::Exception ("Illegal attempt to close page : [" + pageName + "] which is not active.");
        }

        // Load from container, or return if already loaded.
        Page *page = getPage (pageName);
        impl->pages.erase (pageName);
        page->destroyUi ();
}

/*--------------------------------------------------------------------------*/

void App::movePage (std::string const &s, std::string const &pageBName)
{
        std::string pageAName = s;

        // Define which page we are moving from and to.
        if (pageAName.empty () && impl->pages.size () > 1) {
                throw Core::Exception ("-> operation failed. If 'from' page is empty, only one active page can be present. Provide 'from' page name.");
        }

        if (pageAName.empty () && impl->pages.size () > 0) {
                pageAName = impl->pages.begin ()->first;
        }

        if (pageAName.empty ()) {
                addPage (pageBName);
                return;
        }

        BOOST_LOG (lg) << pageAName << "->" << pageBName;

        // Load from container, or return if already loaded.
        Page *pageA = getPage (pageAName);
        Page *pageB = getPage (pageBName);

        if (impl->pages.find (pageBName) != impl->pages.end ()) {
                throw Core::Exception ("Illegal attempt to open multiple instances of page : [" + pageBName + "]");
        }

        impl->pages[pageBName] = pageB;

        if (impl->pages.find (pageAName) == impl->pages.end ()) {
                throw Core::Exception ("Page [" + pageAName + "] is not loaded curently, so it cannot be moved to page [" + pageBName + "].");
        }

        impl->pages.erase (pageAName);

        // Make sure  the entire pageB is loaded (i.e. all its elements that is : its view and its tiles.
        pageB->loadUi (this);

        SlotVector const &slotsA = pageA->getSlots ();
        SlotVector const &slotsB = pageB->getSlots ();

        // Find out common views.
        GtkView *mainViewA = pageA->getView ();
        GtkView *mainViewB = pageB->getView ();

        if (!mainViewB) {
                throw Core::Exception ("view property of object Page is NULL.");
        }

        pageB->reparent (&impl->context);

/*
 * W ogóle, to GtkBuilder ZAWSZE zwroci tą samą instancję obiektu (po ID).
 * Nawet kiedy ten obiekt zniszczymy, to będzie zwracał wskaźnik to tego zniszczonego. Jedyny
 * sposób, żeby zniszczyć i stworzyć obiekt na nowo, to zniszczyć i stworzyć cały GtkBuilder, ale
 * wtedy zniszczymy też i inne obiekty!. Czyli to by dobrze działo, gdyby każde okno było w osobnym
 * pliku, ale to by trzeba było jakoś sprytnie wymusić, na przykład żeby GtkView i GtkTile miały
 * propery ui, gdzie by się podawało nazwę pliku. I wtedy nawet gdyby ktoś robił tak jak my wszystko
 * w jednym pliku (wszystkie okna), to taki XML ładował by się tyle razy ile zdefiniowano GtkView i
 * GtkTie.
 */

        // Find out tiles that are not needed anymore.
        for (Slot *slotA : slotsA) {
                GtkTile *tileA = slotA->getTile ();

                bool tileAPresentInPageB = false;
                for (Slot *slotB : slotsB) {
                        GtkTile *tileB = slotB->getTile ();
                        if (tileB == tileA) {
                                tileAPresentInPageB = true;
                                break;
                        }
                }

                if (!tileAPresentInPageB) {
                        tileA->destroyUi ();
                }
        }

        if (mainViewA != mainViewB) {
                mainViewA->destroyUi();
                mainViewB->show();
        }
}

/*--------------------------------------------------------------------------*/

void App::start (std::string const &unitName)
{
        impl->unitToStart = unitName;
}

/*--------------------------------------------------------------------------*/

void App::back ()
{
        if (impl->previousUnits.size () >= 2) {
                impl->previousUnits.pop ();
        }

        impl->unitToStart = impl->previousUnits.top ();
}

/*--------------------------------------------------------------------------*/

void App::join (std::string const &unitName)
{
        impl->unitsToJoin.insert (unitName);
}

/*--------------------------------------------------------------------------*/

void App::split (std::string const &unitName)
{
        impl->unitsToSplit.insert (unitName);
}

/*--------------------------------------------------------------------------*/

std::pair <IView *, Page *> App::Impl::getActiveViewOrThrow (std::string const &viewName)
{
        IView *view = 0;
        Page *page = 0;

        for (auto elem : pages) {
                page = elem.second;
                view = page->getView ();

                if (view->getName () == viewName) {
                        break;
                }

                SlotVector const &slots = page->getSlots ();
                for (Slot *slot : slots) {
                        GtkTile *tile = slot->getTile ();

                        if (tile->getName () == viewName) {
                                view = tile;
                                break;
                        }
                }
        }

        // Throw an error if no such view was found
        if (!view) {
                std::ostringstream o;
                o << "You requested submit from view which is not currently loaded. Available pages and their views are :\n";

                for (auto elem : pages) {
                        o << elem.second << "\n";
                }

                throw Core::Exception (o.str ());
        }

        return std::make_pair (view, page);
}

/*--------------------------------------------------------------------------*/

void App::submit (std::string const &viewName, std::string const &dataRange, std::string const &controllerName)
{
        std::unique_ptr <SubmitEvent> event {new SubmitEvent};
        event->viewName = viewName;
        event->dataRange = dataRange;
        event->controllerName = controllerName;
        impl->events.push (std::move (event));
}

/*--------------------------------------------------------------------------*/

void App::refresh (std::string const &viewName, std::string const &dataRange)
{
        std::unique_ptr <RefreshEvent> event {new RefreshEvent};
        event->viewName = viewName;
        event->dataRange = dataRange;
        impl->events.push (std::move (event));
}

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
        BOOST_LOG (lg) << "SubmitEvent::run. viewName : [" << event->viewName << "], dataRange : [" << event->dataRange << "], controllerName : [" << event->controllerName << "].";

        /*
         * Potrzebne dane:
         * - Z jakiego widoku ściągamy dane (bo może byćwięcej niż jeden widok w Page..
         *
         * - jakie dane. Konwertuje dane z formularza do modelu. Konweruje całość, lub tylko część. Decyzja jest podejmowana
         * na podstawie jakichś dodatiowych parametrów. Np jezeli guzik OK ma calback $submit() (bez parametrów),
         * to konwerujemy całość. Jeżeli ma tak : $submit ('form'), to konwertowane jest kazde pole które zaczyna
         * się od "form." bo mogą być węcej niż 1 obiektów formularza). Jeżeli damyh $submit ('form.login'), to
         * tylko login. Moznaby się pokusić jeszcze o listę typu : $submit (['form.a', 'form.b']).
         *
         * Na przykłąad domyślnie jeśli mamy GtkContainer o nazwie loginFormm, a w nim pola o nazwach form.login i form.password,
         * a na końcu (w tym samym kontenerze) guzik z podpiętą akcją $app->submit () (czy podobnie), to
         *
         * - Jakiemu kontrolerowi wuwołąć onSubmit : nazwa kontrolera brana z GtkContainer.
         */
        IController *controller = impl->unit.getController (event->controllerName);

        if (!controller) {
                throw Core::Exception ("You requested submit to a controller which is not currently loaded. Controller name : [" + event->controllerName + "].");
        }

        controller->clearFlashScope ();
        controller->getValidationResults ().clear ();
        impl->context.setCurrentController (controller);

        std::pair <IView *, Page *> ret = impl->getActiveViewOrThrow (event->viewName);
        IView *v = ret.first;
        Page *page = ret.second;
        MappingMap const &mappings = page->getMappingsByInput ();

#if 0
        v->printStructure ();
#endif

        GtkView::InputMap inputMap = v->getInputs (event->dataRange);
        bool hasErrors = false;

        for (auto elem : inputMap) {
                std::string inputName = elem.first;
                std::string property = getDefaultProperty ("");
                std::string modelName = inputName;

                MappingDTO dto;
                dto.app = this;
                dto.m2vModelObject = Core::Variant (&impl->context.getSingleFlashAccessor ());
                dto.v2mModelObject = Core::Variant (&impl->context.getSingleFlashAccessor ());
                dto.dataRange = event->dataRange;

                ViewElementDTO elementDTO {G_OBJECT (elem.second)};
                dto.viewElement = &elementDTO;

                MappingMap::const_iterator i;
                if ((i = mappings.find (inputName)) != mappings.end ()) {
                        IMapping *mapping = i->second;
                        ValidationAndBindingResult vr = mapping->view2Model (&dto);
                        controller->getValidationResults ().add (vr);

                        if (!vr.valid) {
                                BOOST_LOG (lg) << "Binding error. Model : [" << vr.model << "]";
                                hasErrors = true;
                        }

                        continue;
                }

                // Default.
                ValidationAndBindingResult vr = Mapping::view2Model (&dto, inputName, "", "");
                controller->getValidationResults ().add (vr);

                if (!vr.valid) {
                        BOOST_LOG (lg) << "Binding error. Model : [" << vr.model << "]";
                        hasErrors = true;
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
        PageDecoratorVector &decorators = page->getDecorators ();

        for (IPageDecorator *decorator : decorators) {
                decorator->run (page, &impl->context);
        }

        if (!hasErrors) {
                std::string nextPage = controller->onSubmit ();
                impl->pagesToShow.insert (nextPage);
        }
}

/*--------------------------------------------------------------------------*/

std::string App::getDefaultProperty (std::string const &widgetType) const
{
        // TODO get this from some configuration / map.
        return "text";
}

/*--------------------------------------------------------------------------*/

void App::doRefresh (RefreshEvent *event)
{
#if 0
        BOOST_LOG (lg) << "RefreshEvent::doRefresh. viewName : [" << event->viewName << "], dataRange : [" << event->dataRange << "].";
#endif

        impl->context.setCurrentController (nullptr);
        typedef std::pair <IView *, Page *> ViewPagePair;
        typedef std::vector <ViewPagePair> ViewPagePairVector;

        ViewPagePairVector viewsToRefresh;

        if (!event->viewName.empty ()) {
                ViewPagePair ret = impl->getActiveViewOrThrow (event->viewName);
                viewsToRefresh.push_back (ret);
        }
        else {
               for (PageMap::value_type const &elem : impl->pages) {
                       Page *page = elem.second;
                       ViewPagePair pair {page->getView (), page};
                       viewsToRefresh.push_back (pair);
               }
        }

        for (ViewPagePair &pair : viewsToRefresh) {
                Page *page = pair.second;
                IView *view = pair.first;
                GtkView::InputMap inputMap = view->getInputs (event->dataRange, true);
                MappingMap const &mappings = page->getMappingsByInput ();

                for (auto elem : inputMap) {
                        std::string inputName = elem.first;

#if 0
                        std::cerr << "++" << inputName << std::endl;
#endif

                        MappingDTO dto;
                        dto.app = this;
                        dto.m2vModelObject = Core::Variant (&impl->context.getAllFlashAccessor ());
                        dto.v2mModelObject = Core::Variant (&impl->context.getAllFlashAccessor ());
                        dto.dataRange = event->dataRange;

                        ViewElementDTO elementDTO {G_OBJECT (elem.second)};
                        dto.viewElement = &elementDTO;

                        MappingMap::const_iterator i;

                        if ((i = mappings.find (inputName)) != mappings.end ()) {
                                IMapping *mapping = i->second;
                                mapping->model2View (&dto);
                                continue;
                        }

                        // Default.
                        Mapping::model2View (&dto, inputName, "", "");
                }

                view->refresh (&impl->context);
        }
}

/*--------------------------------------------------------------------------*/

void App::createContainer (std::string const &configFile)
{
        Ptr <MetaContainer> metaContainer = CompactMetaService::parseFile (configFile);
        impl->container = ContainerFactory::create (metaContainer, true);

//                impl->container->addConversion (typeid (Geometry::Point), Geometry::stringToPointVariant);
//                impl->container->addConversion (typeid (Geometry::Point3), Geometry::stringToPoint3Variant);
//                impl->container->addConversion (typeid (Geometry::LineString), Geometry::stringToLineStringVariant);
//                impl->container->addConversion (typeid (Model::HAlign), Model::stringToHAlign);
//                impl->container->addConversion (typeid (Model::VAlign), Model::stringToVAlign);
//                impl->container->addConversion (typeid (Model::HGravity), Model::stringToHGravity);
//                impl->container->addConversion (typeid (Model::VGravity), Model::stringToVGravity);
//                impl->container->addConversion (typeid (Model::LinearGroup::Type), Model::stringToLinearGroupType);

//        impl->container->addSingleton (i->first.c_str (), i->second);

        ContainerFactory::init (impl->container.get (), metaContainer.get ());
        impl->config = vcast <Config *> (impl->container->getBean ("config"));
}

/*--------------------------------------------------------------------------*/

Context &App::getContext ()
{
        return impl->context;
}

/*--------------------------------------------------------------------------*/

k202::K202 *App::getK202 ()
{
        static Ptr <k202::K202> k202 = k202::K202::create ();
        return k202.get ();
}

/*--------------------------------------------------------------------------*/

struct BWInitializer {

        BWInitializer ()
        {
                beanWrapper = new Wrapper::BeanWrapper (true);
//                beanWrapper->addPlugin (new GObjectWrapperPlugin);
                beanWrapper->addPlugin (new Wrapper::PropertyRWBeanWrapperPlugin ());
                beanWrapper->addPlugin (new Wrapper::GetPutMethodRWBeanWrapperPlugin ());
                beanWrapper->addPlugin (new Wrapper::MethodPlugin (Wrapper::MethodPlugin::METHOD));

                Editor::TypeEditor *typeEditor = new Editor::TypeEditor (true);
                typeEditor->setEqType (new Editor::NoopEditor ());
                typeEditor->setNullType (new Editor::NoopEditor ());

                typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (int), new Editor::StreamEditor <std::string, int> ()));
                typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (double), new Editor::LexicalEditor <std::string, double> ()));
                typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (float), new Editor::LexicalEditor <std::string, float> ()));
                typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (char), new Editor::LexicalEditor <std::string, char> ()));
                typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (bool), new Editor::LexicalEditor <std::string, bool> ()));
                typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (unsigned int), new Editor::StreamEditor <std::string, unsigned int> ()));
                typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (unsigned char), new Editor::StreamEditor <std::string, unsigned char> ()));
                typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (long), new Editor::StreamEditor <std::string, long> ()));
                typeEditor->addType (Editor::TypeEditor::Type (typeid (std::string), typeid (unsigned long), new Editor::StreamEditor <std::string, unsigned long> ()));

                Editor::ChainEditor *chain = new Editor::ChainEditor (true);
                chain->addEditor (typeEditor);
                chain->addEditor (new Editor::NoopEditor ());

                beanWrapper->setEditor (chain);
        }

        Wrapper::BeanWrapper *beanWrapper = 0;

};

/*--------------------------------------------------------------------------*/

Wrapper::BeanWrapper *App::getBeanWrapper ()
{
        return App::Impl::getBeanWrapper ();
}

/*--------------------------------------------------------------------------*/

Wrapper::BeanWrapper *App::Impl::getBeanWrapper ()
{
        static BWInitializer bwInitializer;
        return bwInitializer.beanWrapper;
}

/*--------------------------------------------------------------------------*/

IUnit *App::getUnit (std::string const &name)
{
        IUnit* unit = ocast <IUnit *> (impl->container->getBean (name));
        return unit;
}

/*--------------------------------------------------------------------------*/

Page *App::getPage (std::string const &name)
{
        Page* page = ocast <Page *> (impl->container->getBean (name));
        return page;
}

/*--------------------------------------------------------------------------*/

IUnit *App::getCurrentUnit ()
{
        return &impl->unit;
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
        App *app = static_cast <App *> (userData);
        app->run ();
        if (app->impl->quitRequested) {
                gtk_main_quit ();
                return G_SOURCE_REMOVE;
        }
        else {
                return G_SOURCE_CONTINUE;
        }
}

} // namespace GtkForms
