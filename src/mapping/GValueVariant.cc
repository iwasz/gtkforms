/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <gtk/gtk.h>
#include "GValueVariant.h"

namespace GtkForms {
using namespace Core;

Variant gValueToVariant (GValue const *gVal)
{
#if 0
        std::cerr << G_VALUE_TYPE_NAME (gVal) << std::endl;
        std::cerr << "Button : " << GTK_TYPE_TOOL_BUTTON << ", " << G_VALUE_TYPE (gVal) << std::endl;
        std::cerr << "G_TYPE_IS_ABSTRACT() : " << G_TYPE_IS_ABSTRACT (G_VALUE_TYPE (gVal)) << std::endl;
        std::cerr << "G_TYPE_IS_DERIVED() : " << G_TYPE_IS_DERIVED(G_VALUE_TYPE (gVal)) << std::endl;
        std::cerr << "G_TYPE_IS_OBJECT() : " << G_TYPE_IS_OBJECT(G_VALUE_TYPE (gVal)) << std::endl;
#endif

        switch (G_TYPE_FUNDAMENTAL(G_VALUE_TYPE (gVal))) {
        case G_TYPE_OBJECT:
        case G_TYPE_INTERFACE:
                return Variant (G_OBJECT (g_value_get_object (gVal)));
        case G_TYPE_STRING:
                return Variant (std::string (g_value_get_string (gVal)));
        case G_TYPE_INT:
                return Variant ((int)g_value_get_int (gVal));
        case G_TYPE_FLOAT:
                return Variant ((float)g_value_get_float (gVal));
        case G_TYPE_DOUBLE:
                return Variant ((double)g_value_get_double (gVal));
        case G_TYPE_POINTER:
                // gpointer
                return Variant (g_value_get_pointer (gVal));
        case G_TYPE_BOOLEAN:
                return Variant ((bool)g_value_get_boolean (gVal));
        case G_TYPE_UINT:
                return Variant ((unsigned int)g_value_get_uint (gVal));
        case G_TYPE_LONG:
                return Variant ((long)g_value_get_long (gVal));
        case G_TYPE_ULONG:
                return Variant ((unsigned long)g_value_get_ulong (gVal));
        case G_TYPE_INT64:
                return Variant ((signed long)g_value_get_int64 (gVal));
        case G_TYPE_UINT64:
                return Variant ((unsigned long)g_value_get_uint64 (gVal));
        case G_TYPE_CHAR:
                return Variant ((char)g_value_get_schar (gVal));
        case G_TYPE_UCHAR:
                return Variant ((unsigned char)g_value_get_uchar (gVal));
        case G_TYPE_ENUM:
                // guint
                return Variant ((int)g_value_get_enum (gVal));
        case G_TYPE_FLAGS:
                // guint
                return Variant ((int)g_value_get_flags (gVal));
        case G_TYPE_BOXED:
                // gpointer
                return Variant (g_value_get_boxed (gVal));
        case G_TYPE_PARAM:
                // GParamSpec*
                return Variant (g_value_get_param (gVal));
        case G_TYPE_INVALID:
        case G_TYPE_NONE:
        default:
                return Core::Variant ();
        }
}

/****************************************************************************/

GValue *variantToGValue (GValue *gVal, Core::Variant const &vVal)
{
        if (!gVal) {
                return gVal;
        }

        switch (vVal.getType ()) {
        case Variant::STRING:
                g_value_init (gVal, G_TYPE_STRING);
                g_value_set_string (gVal, vcast <std::string> (vVal).c_str ()); // Tu chyba się zrobi kopia.
                break;
        case Variant::STRING_POINTER:
                g_value_init (gVal, G_TYPE_STRING);
                g_value_set_string (gVal, vcast <std::string> (vVal).c_str ());
                break;
        case Variant::STRING_POINTER_CONST:
                g_value_init (gVal, G_TYPE_STRING);
                g_value_set_string (gVal, vcast <std::string> (vVal).c_str ());
                break;
        case Variant::POINTER:
                g_value_init (gVal, G_TYPE_POINTER);
                g_value_set_pointer (gVal, vcast <void *> (vVal));
                break;
        case Variant::SMART:
                g_value_init (gVal, G_TYPE_POINTER);
                g_value_set_pointer (gVal, vcast <void *> (vVal));
                break;
                break;
        case Variant::OBJECT:
                g_value_init (gVal, G_TYPE_POINTER);
                g_value_set_pointer (gVal, vcast <void *> (vVal));
                break;
        case Variant::SMART_OBJECT:
                g_value_init (gVal, G_TYPE_POINTER);
                g_value_set_pointer (gVal, vcast <void *> (vVal));
                break;
        case Variant::BOOL:
                g_value_init (gVal, G_TYPE_BOOLEAN);
                g_value_set_boolean (gVal, vcast <bool> (vVal));
                break;
        case Variant::CHAR:
                g_value_init (gVal, G_TYPE_CHAR);
                g_value_set_schar (gVal, vcast <char> (vVal));
                break;
        case Variant::UNSIGNED_CHAR:
                g_value_init (gVal, G_TYPE_UCHAR);
                g_value_set_uchar (gVal, vcast <unsigned char> (vVal));
                break;
        case Variant::DOUBLE:
                g_value_init (gVal, G_TYPE_DOUBLE);
                g_value_set_double (gVal, vcast <double> (vVal));
                break;
        case Variant::LONG_DOUBLE:
                g_value_init (gVal, G_TYPE_DOUBLE);
                g_value_set_double (gVal, vcast <long double> (vVal));
                break;
        case Variant::FLOAT:
                g_value_init (gVal, G_TYPE_FLOAT);
                g_value_set_float (gVal, vcast <float> (vVal));
                break;
        case Variant::INT:
                g_value_init (gVal, G_TYPE_INT);
                g_value_set_int (gVal, vcast <int> (vVal));
                break;
        case Variant::UNSIGNED_INT:
                g_value_init (gVal, G_TYPE_UINT);
                g_value_set_uint (gVal, vcast <unsigned int> (vVal));
                break;
        case Variant::LONG_INT:
                g_value_init (gVal, G_TYPE_LONG);
                g_value_set_long (gVal, vcast <long int> (vVal));
                break;
        case Variant::UNSIGNED_LONG_INT:
                g_value_init (gVal, G_TYPE_ULONG);
                g_value_set_long (gVal, vcast <unsigned long int> (vVal));
                break;
        case Variant::SHORT_INT:
                g_value_init (gVal, G_TYPE_INT);
                g_value_set_int (gVal, vcast <short int> (vVal));
                break;
        case Variant::UNSIGNED_SHORT_INT:
                g_value_init (gVal, G_TYPE_UINT);
                g_value_set_uint (gVal, vcast <unsigned short int> (vVal));
                break;
        case Variant::NIL:
                g_value_init (gVal, G_TYPE_POINTER);
                g_value_set_pointer (gVal, 0);
                break;
        case Variant::NONE:
        default:
                return NULL;
                break;
        }

        return gVal;
}

}
