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
#include "controller/IEvent.h"
#include "controller/SubmitEvent.h"
#include "controller/QuitEvent.h"
#include "controller/IController.h"

using namespace Container;

/**
 *
 */
struct App::Impl {

        Unit *getUnit (std::string );

        Core::StringVector unitsToStart;
        Core::StringVector unitsToJoin;
        Core::StringVector unitsToSplit;

        Core::StringVector pagesToHide;
        Core::StringVector pagesToShow;

        EventStack events;

        // Current unit.
        Unit unit;
        // Current page;
        Page page;

        Ptr <BeanFactoryContainer> container;
};

App::App (std::string const &configurationFile)
{
        impl = new Impl;
        createContainer (configurationFile);
}

App::~App ()
{
        delete impl;
}

void App::run ()
{
        src::logger_mt& lg = logger::get();
        BOOST_LOG (lg) << "Greetings from the global logger!";

        UnitOperationResult result;

        for (std::string const &unitName : impl->unitsToStart) {
                Unit *unit = getUnit (unitName);
                result += impl->unit.replace (unit);
        }

        for (std::string const &unitName : impl->unitsToJoin) {
                Unit *unit = getUnit (unitName);
                result += impl->unit.add (unit);
        }

        for (std::string const &unitName : impl->unitsToSplit) {
                Unit *unit = getUnit (unitName);
                result += impl->unit.remove (unit);
        }

        impl->unitsToStart.clear ();
        impl->unitsToJoin.clear ();
        impl->unitsToSplit.clear ();

        // Now that our Unit is up to date, w can deal with form submission and data conversion
        // events:
        // - submit
        // - quit

        while (!impl->events.empty ()) {
                std::unique_ptr <IEvent> event = std::move (impl->events.top ());
                impl->events.pop ();
                event->run (this);
        }


        // Here we are dealing with pages and thier views to hide.

        for (ControllerMap::value_type const &entry : result.removed) {
                IController *controller = entry.second;
                std::string pageName = controller->end ();

                if (!pageName.empty ()) {
                        impl->pagesToHide.push_back (pageName);
                }
        }

        ViewMap viewsToHide;
        for (std::string const &pageName : impl->pagesToHide) {
                Page *page = getPage (pageName);
                ViewMap tmp = impl->page.remove (page);
                std::copy (tmp.begin (), tmp.end (), std::inserter (viewsToHide, viewsToHide.end ()));
        }

        for (ViewMap::value_type const &entry : viewsToHide) {
                IView *view = entry.second;
                view->hide ();
        }

        // All views that was meant to be closed are colsed here. Now show new views user has requested.

        for (ControllerMap::value_type const &entry : result.added) {
                IController *controller = entry.second;
                std::string pageName = controller->end ();

                if (!pageName.empty ()) {
                        impl->pagesToShow.push_back (pageName);
                }
        }

        ViewMap viewsToShow;
        for (std::string const &pageName : impl->pagesToShow) {
                Page *page = getPage (pageName);
                ViewMap tmp = impl->page.replace (page);
                std::copy (tmp.begin (), tmp.end (), std::inserter (viewsToShow, viewsToShow.end ()));
        }

        for (ViewMap::value_type const &entry : viewsToShow) {
                IView *view = entry.second;
                // TODO model2View (); TODO pytanie, ale konwertować z modelu na widok wszytsko, czy tylko to co się zmieniło?
                view->show ();
        }


        /*
                currentController = container->getBean(controllerName);

                // To by robił Flow.
                std::string viewName = currentController->start();
                currentView = ViewResolver.getView(viewName);
                currentView->model2View();
                currentView->show();
        */

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
        impl->pagesToShow.push_back (nextPage);
}

void App::start (std::string const &unitName)
{
        impl->unitsToStart.push_back (unitName);
}

void App::join (std::string const &unitName)
{
        impl->unitsToJoin.push_back (unitName);
}

void App::split (std::string const &unitName)
{
        impl->unitsToSplit.push_back (unitName);
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
