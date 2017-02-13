/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GVALUE_VARIANT_GTKFORMS_H_
#define GVALUE_VARIANT_GTKFORMS_H_

#include <Tiliae.h>
#include <gtk/gtk.h>

namespace GtkForms {

/**
 * Converts GValue to Core::Variant.
 */
extern Core::Variant gValueToVariant (GValue const *gVal);

/**
 * Converts Core::Variant to GValue. Returned GValue must be unset after use to free
 * memory. Use g_value_unset (&val);
 */
extern GValue *variantToGValue (GValue *gVal, Core::Variant const &);
}

#endif /* GTKFORMS_H_ */
