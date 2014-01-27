/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "AbstractController.h"
#include "App.h"
#include "Context.h"

Core::VariantMap &AbstractController::getSessionScope ()
{
        app->getContext ().getSessionScope ();
}

/*--------------------------------------------------------------------------*/

Core::VariantMap &AbstractController::getUnitScope ()
{
        app->getContext ().getUnitScope ();
}

/*--------------------------------------------------------------------------*/

Core::VariantMap &AbstractController::getFlashScope ()
{
        app->getContext ().getFlashScope ();
}
