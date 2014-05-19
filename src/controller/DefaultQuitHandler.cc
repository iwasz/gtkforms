/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "DefaultQuitHandler.h"
#include "App.h"

namespace GtkForms {

void DefaultQuitHandler::run (App *app)
{
        app->quit ();
}

} /* namespace GtkForms */
