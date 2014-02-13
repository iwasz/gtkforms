/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <GtkForms.h>
#include <gtk/gtk.h>

int main (int argc, char **argv)
{
        gtk_init (&argc, &argv);
        GtkForms::App app {"../demo/first/config.xml"};

        app.start ("login");
//        app.join ("main");
//        app.split ("main");
//        app.start ("result");

        gtk_main ();

}


