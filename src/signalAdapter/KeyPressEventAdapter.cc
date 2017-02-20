/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "KeyPressEventAdapter.h"
#include <core/Exception.h>
#include <gtk/gtk.h>

namespace GtkForms {

Core::VariantVector KeyPressEventAdapter::adapt (guint n_param_values, const GValue *param_values) const
{
        GValue const *gValue = param_values;
        if (!G_VALUE_HOLDS_OBJECT (gValue)) {
                throw Core::Exception ("GValue not object.");
        }

        GObject *object = G_OBJECT (g_value_get_object (gValue));
        GtkWidget *widget = GTK_WIDGET (object);

        gValue = param_values + 1;
        if (!G_VALUE_HOLDS_BOXED (gValue)) {
                throw Core::Exception ("GValue not boxed.");
        }

        gpointer boxedPtr = g_value_get_boxed (gValue);
        GdkEvent *event = static_cast<GdkEvent *> (boxedPtr);

        Core::VariantVector params;
        params.push_back (Core::Variant (event->key.keyval));
        params.push_back (Core::Variant (event->key.state));
        params.push_back (Core::Variant (widget));
        return params;
}

} // namespace
