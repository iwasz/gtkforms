/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <cassert>
#include "App.h"
#include "../uiFactory/IUIFactory.h"

/****************************************************************************/

void App::run ()
{
        GtkWidget *widget = factory->create ();
        assert (widget);

        gtk_widget_show (widget);
        gtk_main ();
}
