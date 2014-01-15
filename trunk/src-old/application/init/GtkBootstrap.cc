/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <gtk/gtk.h>
#include "GtkBootstrap.h"

namespace GtkForms {

void GtkBootstrap::init (int *argc, char ***argv)
{
        gtk_init (argc, argv);
}

}
