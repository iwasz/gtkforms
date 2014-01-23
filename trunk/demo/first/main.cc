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

        App app {"../demo/first/config.xml"};
        app.start ("login");
        app.run ();

        gtk_main ();
}


