/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <Tiliae.h>
#include "App.h"
#include "Unit.h"

/**
 *
 */
struct App::Impl {

        Unit *getUnit (std::string );

        Core::StringVector unitsToStart;
        Core::StringVector unitsToJoin;
        Core::StringVector unitsToSplit;

        // Current unit.
        Unit unit;
};

App::App (std::string const &configurationFile)
{
        impl = new Impl;
}

App::~App ()
{
        delete impl;
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

void App::run ()
{
        for (std::string const &unitName : impl->unitsToStart) {
                Unit *unit = getUnit (unitName);
                ControllerVector fresh = impl->unit.add (unit);
        }

        // Do sth with units
        impl->unitsToStart.clear ();
        impl->unitsToJoin.clear ();
        impl->unitsToSplit.clear ();



        /*
                currentController = container->getBean(controllerName);

                // To by robił Flow.
                std::string viewName = currentController->start();
                currentView = ViewResolver.getView(viewName);
                currentView->model2View();
                currentView->show();
        */

}
