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
#include "mapping/GObjectWrapperPlugin.h"

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
        Context context;
        GtkTileManager tileManager;
        bool model2ViewRequest = false;
};

/*--------------------------------------------------------------------------*/

App::App (std::string const &configurationFile)
{
        impl = new Impl;
        createContainer (configurationFile);
        g_idle_add (guiThread, static_cast <gpointer> (this));
        impl->context.getSessionScope ()["app"] = Core::Variant (*this);
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
                std::string command = controller->end ();
//                impl->context.getSessionScope ().erase (controller->getName ());
                impl->context.getSessionScope ().erase (entry.first);

                if (!command.empty ()) {
                        viewCommands.insert (command);
                }
        }

        for (ControllerMap::value_type const &entry : uoResult.added) {
                IController *controller = entry.second;
                controller->setApp (this);
                std::string command = controller->start ();
                impl->model2ViewRequest = true;
//                impl->context.getSessionScope ()[controller->getName ()] = Core::Variant (controller);
                impl->context.getSessionScope ()[entry.first] = Core::Variant (controller);

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
                                impl->model2ViewRequest = true;
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

                                impl->model2ViewRequest = true;
                        }
                        else {
                                throw Core::Exception ("Invalid view command. Use +, - or -> operations.");
                        }

                }
        }
}

/*--------------------------------------------------------------------------*/

void App::run ()
{
        Core::StringSet viewCommands = manageUnits ();
        managePages (viewCommands);

        if (impl->model2ViewRequest) {
                // Model -> view conversion
        }

        impl->model2ViewRequest = false;
        usleep (MAIN_LOOP_USLEEP);
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

        SlotVector slots = page->getSlots ();
        mainView->reparent (slots, &impl->context);
        // TODO jakoś przenieść!
//        guiLoadTheme ("/home/iwasz/.themes/", "BioMorph", G_OBJECT (mainView));
        mainView->show ();
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
                // TODO What should happen here? Nothing? Exception? Another instance of view?
                throw Core::Exception ("Illegal attempt to open multiple instances of page : [" + pageBName + "]");
        }

        impl->pages[pageBName] = pageB;

        if (impl->pages.find (pageAName) == impl->pages.end ()) {
                throw Core::Exception ("Page [" + pageAName + "] is not loaded curently, so it cannot be moved to page [" + pageBName + "].");
        }

        impl->pages.erase (pageAName);

        // Load UI file, or noop if loaded.
        pageB->loadUi (this);

        // Get tiles and add them.
//        GtkTileMap tilesA = pageA->getTiles ();
//        GtkTileMap tilesB = pageB->getTiles ();

//        TODO!!!!!
//        // Find out common tiles.
//        for (auto elem : tilesA) {
//                std::string tileAName = elem.first;
//                GtkTile *tileA = elem.second;
//
//                // Add only those tiles, that are not present in tilesB map.
//                GtkTileMap::iterator i;
//                if ((i = tilesB.find (tileAName)) == tilesB.end ()) {
//                        tilesB[tileAName] = tileA;
//                }
//        }

        // Find out common views.
        GtkView *mainViewA = pageA->getView ();
        GtkView *mainViewB = pageB->getView ();

        if (!mainViewB) {
                throw Core::Exception ("view property of object Page is NULL.");
        }

        SlotVector slots = pageB->getSlots ();

        mainViewB->reparent (slots, &impl->context);

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

        std::pair <IView *, Page *> ret = impl->getActiveViewOrThrow (event->viewName);
        GtkView *v = dynamic_cast <GtkView *> (ret.first);
        Page *page = ret.second;
        MappingMap const &mappings = page->getMappingsByInput ();

#if 0
        v->printStructure ();
#endif

        GtkView::InputMap inputMap = v->getInputs (event->dataRange);

        for (auto elem : inputMap) {
                std::string inputName = elem.first;
                std::string property = getDefaultProperty ("");
                std::string modelName = inputName;

                MappingDTO dto;
                dto.app = this;
                dto.inputWidget = G_OBJECT (elem.second);
                dto.context = &impl->context;
                dto.dataRange = event->dataRange;

                MappingMap::const_iterator i;
                if ((i = mappings.find (inputName)) != mappings.end ()) {
                        IMapping *mapping = i->second;
                        mapping->view2Model (&dto);
                        continue;
                }

                // Default.
                Mapping::view2Model (&dto, inputName, "", "");
        }

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

        std::string nextPage = controller->onSubmit ();
        impl->pagesToShow.insert (nextPage);
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
        BOOST_LOG (lg) << "RefreshEvent::doRefresh. viewName : [" << event->viewName << "], dataRange : [" << event->dataRange << "].";

        PageMap refreshPages;
        if (!event->viewName.empty ()) {
                std::pair <IView *, Page *> ret = impl->getActiveViewOrThrow (event->viewName);
                Page *page = ret.second;
                refreshPages[page->getName ()] = page;
        }
        else {
               refreshPages = impl->pages;
        }

        for (auto elem : refreshPages) {
                Page *page = elem.second;
                // ? why dynamic_cast!
                GtkView *v = dynamic_cast <GtkView *> (page->getView ());
                GtkView::InputMap inputMap = v->getInputs (event->dataRange);
                MappingMap const &mappings = page->getMappingsByInput ();

                for (auto elem : inputMap) {
                        std::string inputName = elem.first;
                        std::string property;
                        std::string modelName = inputName;

                        MappingDTO dto;
                        dto.app = this;
                        dto.inputWidget = G_OBJECT (elem.second);
                        dto.context = &impl->context;
                        dto.dataRange = event->dataRange;

                        MappingMap::const_iterator i;

                        if ((i = mappings.find (inputName)) != mappings.end ()) {
                                IMapping *mapping = i->second;
                                mapping->model2View (&dto);
                                continue;
                        }

                        // Default.
                        Mapping::model2View (&dto, inputName, "", "");
                }
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
//        impl->config = vcast <U::Config *> (impl->container->getBean ("config"));
//        config (impl->config);
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
                beanWrapper->addPlugin (new GObjectWrapperPlugin);
                beanWrapper->addPlugin (new Wrapper::PropertyRWBeanWrapperPlugin ());
                beanWrapper->addPlugin (new Wrapper::GetPutMethodRWBeanWrapperPlugin ());
                beanWrapper->addPlugin (new Wrapper::MethodPlugin (Wrapper::MethodPlugin::METHOD));
        }

        Wrapper::BeanWrapper *beanWrapper = 0;

};

/*--------------------------------------------------------------------------*/

Wrapper::BeanWrapper *App::getBeanWrapper ()
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

gboolean guiThread (gpointer userData)
{
        App *app = static_cast <App *> (userData);
        app->run ();
        return G_SOURCE_CONTINUE;
}

} // namespace GtkForms
