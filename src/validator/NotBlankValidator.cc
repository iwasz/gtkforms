/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <boost/algorithm/string/trim.hpp>
#include "NotBlankValidator.h"
#include "Context.h"

namespace GtkForms {

ValidationResult NotBlankValidator::validate (Context &ctx) const
{
        Core::Variant v = ctx.get (getModel ());

        if (v.isNone () || v.isNull ()) {
                return createResult (false, "Wypełnij pole");
        }

        if (ccast <std::string> (v)) {
                std::string s = vcast <std::string> (v);
                boost::trim (s);
                if (s.empty ()) {
                        return createResult (false, "Wypełnij pole");
                }
        }

        return createResult ();
}

} /* namespace GtkForms */
