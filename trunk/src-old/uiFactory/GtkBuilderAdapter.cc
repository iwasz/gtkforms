/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "GtkBuilderAdapter.h"

GtkBuilderAdapter::GtkBuilderAdapter ()
{
        builder = gtk_builder_new ();
}

GtkBuilderAdapter::~GtkBuilderAdapter ()
{
        g_object_unref (builder);
}
