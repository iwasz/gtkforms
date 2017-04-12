/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "FileChooserAdapter.h"
#include <core/Exception.h>
#include <gtk/gtk.h>

namespace GtkForms {

Core::VariantVector FileChooserAdapter::adapt (guint n_param_values, const GValue *param_values) const
{
        GValue const *gValue = param_values;
        if (!G_VALUE_HOLDS_OBJECT (gValue)) {
                throw Core::Exception ("GValue not object.");
        }

        GObject *object = G_OBJECT (g_value_get_object (gValue));
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (object);

        gValue = param_values + 1;
        if (!G_VALUE_HOLDS_INT (gValue)) {
                throw Core::Exception ("GValue not INT.");
        }

        gint responseId = g_value_get_int (gValue);
        char const *filename = gtk_file_chooser_get_filename (chooser);

        Core::VariantVector params;
        params.push_back (Core::Variant (responseId));
        params.push_back (Core::Variant (filename));
        return params;
}

} // namespace
