/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <cassert>
#include "App.h"
#include "../view/IUIFactory.h"

namespace GtkForms {

/****************************************************************************/

void App::run ()
{
#if 0
        GtkWidget *widget = factory->create ();
        assert (widget);
        gtk_widget_show (widget);
#endif

#if 0
        std::cerr << "App::run" << std::endl;
#endif

        gtk_main ();
}

}
