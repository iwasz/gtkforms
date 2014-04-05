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

ValidationAndBindingResult NotBlankValidator::validate (Context &ctx) const
{
        Core::Variant v = ctx.get (getModel ());

        if (v.isNone () || v.isNull ()) {
                return createResult (false);
        }

        if (ccast <std::string> (v)) {
                std::string s = vcast <std::string> (v);
                boost::trim (s);
                if (s.empty ()) {
                        return createResult (false);
                }
        }

        return createResult (true);
}

} /* namespace GtkForms */
