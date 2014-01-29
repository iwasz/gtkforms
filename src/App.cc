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
#include "Logging.h"
#include "Context.h"
#include "controller/IEvent.h"
#include "controller/SubmitEvent.h"
#include "controller/QuitEvent.h"
#include "controller/IController.h"
#include "view/GtkTileManager.h"

namespace GtkForms {
using namespace Container;

gboolean guiThread (gpointer user_data);

/**
 *
 */
struct App::Impl {

        Unit *getUnit (std::string );

        std::string unitToStart;
        Core::StringSet unitsToJoin;
        Core::StringSet unitsToSplit;

        Core::StringSet pagesToHide;
        Core::StringSet pagesToShow;

        EventStack events;

        // Current unit.
        Unit unit;
        // Current page;
        Page page;

        Ptr <BeanFactoryContainer> container;
        Context context;
        GtkTileManager tileManager;
};

App::App (std::string const &configurationFile)
{
        impl = new Impl;
        createContainer (configurationFile);
        g_idle_add (guiThread, static_cast <gpointer> (this));
}

App::~App ()
{
        delete impl;
}

void App::run ()
{
        src::logger_mt& lg = logger::get();

        UnitOperationResult uoResult;

        if (!impl->unitToStart.empty ()) {
                IUnit *unit = getUnit (impl->unitToStart);
                uoResult += impl->unit.start (unit);
        }

        for (std::string const &unitName : impl->unitsToJoin) {
                IUnit *unit = getUnit (unitName);
                uoResult += impl->unit.join (unit);
        }

        for (std::string const &unitName : impl->unitsToSplit) {
                IUnit *unit = getUnit (unitName);
                uoResult += impl->unit.split (unit);
        }

//        BOOST_LOG (lg) << impl->unit;

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

        // Here we are dealing with pages and thier views to hide.
        for (ControllerMap::value_type const &entry : uoResult.removed) {
                IController *controller = entry.second;
                controller->setApp (this);
                std::string pageName = controller->end ();

                if (!pageName.empty ()) {
                        impl->pagesToHide.insert (pageName);
                }
        }

        PageOperationResult poResult;
        for (std::string const &pageName : impl->pagesToHide) {
                IPage *page = getPage (pageName);
                poResult += impl->page.split (page);
        }

        for (ViewMap::value_type const &entry : poResult.removed) {
                IView *view = entry.second;
                view->destroy ();
        }

        // All views that was meant to be closed are colsed here. Now show new views user has requested.

        for (ControllerMap::value_type const &entry : uoResult.added) {
                IController *controller = entry.second;
                controller->setApp (this);
                std::string pageName = controller->start ();

                if (!pageName.empty ()) {
                        impl->pagesToShow.insert (pageName);
                }
        }

        bool started = false; // Protection against multiple starts (second page to be started would replace the first).
        std::string firstPageToStart; // For more maningful exception message.
        for (std::string const &pageName : impl->pagesToShow) {
                IPage *page = getPage (pageName);

                if (page->getJoinable ()) {
                        poResult += impl->page.join (page);
                }
                else {
                        if (started) {
                                throw Core::Exception ("You are about to start two pages. Only one page can be started at a time. Other pages must be joined. First page started : [" + firstPageToStart + "], second page you tried to start : [" + pageName + "].");
                        }

                        poResult += impl->page.start (page);
                        started = true;
                        firstPageToStart = pageName;
                }
        }

        impl->tileManager.reparent (&poResult.added, true);
//        impl->tileManager.show (poResult.added);

//        for (ViewMap::value_type const &entry : poResult.added) {
//                IView *view = entry.second;
//                // TODO model2View (); TODO pytanie, ale konwertować z modelu na widok wszytsko, czy tylko to co się zmieniło?
//                view->show ();
//        }

//        BOOST_LOG (lg) << impl->page;

        /*
                currentController = container->getBean(controllerName);

                // To by robił Flow.
                std::string viewName = currentController->start();
                currentView = ViewResolver.getView(viewName);
                currentView->model2View();
                currentView->show();
        */

        usleep (MAIN_LOOP_USLEEP);
}


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
void App::doSubmit (std::string const &viewName, std::string const &dataRange, std::string const &controllerName)
{
        /*
         */

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
        IView *view = impl->page.getView (viewName);

        if (!view) {
                throw Core::Exception {"No such view : [" + viewName + "]."};
        }

        IController *controller = impl->unit.getController (controllerName);

        if (!controller) {
                throw Core::Exception {"No such controller : [" + controllerName + "]."};
        }

        view->view2Model (dataRange);

        std::string nextPage = controller->onSubmit();
        impl->pagesToShow.insert (nextPage);
}

void App::start (std::string const &unitName)
{
        impl->unitToStart = unitName;
}

void App::join (std::string const &unitName)
{
        impl->unitsToJoin.insert (unitName);
}

void App::split (std::string const &unitName)
{
        impl->unitsToSplit.insert (unitName);
}

void App::submit (std::string const &controllerName, std::string const &formName)
{
        impl->events.push (std::unique_ptr <IEvent> (new SubmitEvent {}));
}

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

Context &App::getContext ()
{
        return impl->context;
}

Context const &App::getContext () const
{
        return impl->context;
}

IUnit *App::getUnit (std::string const &name)
{
        IUnit* unit = ocast <IUnit *> (impl->container->getBean (name));
        return unit;
}

IPage *App::getPage (std::string const &name)
{
        IPage* page = ocast <IPage *> (impl->container->getBean (name));
        return page;
}

gboolean guiThread (gpointer userData)
{
        App *app = static_cast <App *> (userData);
        app->run ();
        return G_SOURCE_CONTINUE;
}

} // namespace GtkForms
