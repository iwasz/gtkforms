/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "GtkBuilderAdapter.h"

namespace GtkForms {

GtkBuilderAdapter::GtkBuilderAdapter ()
{
#if 0
        std::cerr << "GtkBuilderAdapter::GtkBuilderAdapter" << std::endl;
#endif
        builder = gtk_builder_new ();
}

/****************************************************************************/

GtkBuilderAdapter::~GtkBuilderAdapter ()
{
        g_object_unref (builder);
}

}
