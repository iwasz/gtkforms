/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKFORMS_H_
#define GTKFORMS_H_

#include <ApiMacro.h>

namespace GtkForms {

/**
 * Converts GValue to Core::Variant.
 */
extern TILIAE_API Core::Variant gValueToVariant (GValue const *gVal);

/**
 * Converts Core::Variant to GValue. Returned GValue must be unset after use to free
 * memory. Use g_value_unset (&val);
 */
extern TILIAE_API GValue *variantToGValue (GValue *gVal, Core::Variant const &);

}

#	endif /* GTKFORMS_H_ */
