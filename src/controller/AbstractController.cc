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

namespace GtkForms {

Core::VariantMap &AbstractController::getSessionScope ()
{
        return app->getContext ().getSessionScope ();
}

/*--------------------------------------------------------------------------*/

Core::VariantMap &AbstractController::getUnitScope ()
{
        return app->getContext ().getUnitScope ();
}

/*--------------------------------------------------------------------------*/

Core::VariantMap &AbstractController::getFlashScope ()
{
        return app->getContext ().getFlashScope ();
}

} // namespace GtkForms
