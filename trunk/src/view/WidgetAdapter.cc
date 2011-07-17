/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "WidgetAdapter.h"
#include "tools/GValueVariant.h"

namespace GtkForms {

Core::Variant WidgetAdapter::get (const std::string &name) const
{
        char const *param = name.c_str ();
        GParamSpec *spec = g_object_class_find_property (G_OBJECT_GET_CLASS (widget), param);

        if (!param) {
                return Core::Variant ();
        }

        GType propType = spec->value_type;
        GValue propValue = {0};

        g_value_init (&propValue, propType);
        g_object_get_property (G_OBJECT (widget), param, &propValue);
        Core::Variant v = gValueToVariant (&propValue);
        g_value_unset (&propValue);

        return v;
}

/****************************************************************************/

void WidgetAdapter::set (const std::string &name, const Core::Variant &object)
{
        GValue gVal = {0};
        variantToGValue (&gVal, object);
        g_object_set_property (G_OBJECT (widget), name.c_str (), &gVal);
        g_value_unset (&gVal);
}

} /* namespace GtkForms */
