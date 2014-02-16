/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#include "RefreshEvent.h"
#include "App.h"

namespace GtkForms {

void RefreshEvent::run (App *app)
{
        app->doRefresh (this);
}

} /* namespace GtkForms */
