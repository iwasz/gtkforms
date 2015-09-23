/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef NOTBLANKVALIDATOR_H_
#define NOTBLANKVALIDATOR_H_

#include "AbstractValidator.h"

namespace GtkForms {

class NotBlankValidator : public AbstractValidator {
public:
        REFLECTION_CONSTRUCTOR_ (void)
        REFLECTION_BASE_CLASS ("AbstractValidator")
        virtual ~NotBlankValidator () {}
        virtual ValidationAndBindingResult validate (Context &ctx) const;
        REFLECTION_END (NotBlankValidator)
};

} /* namespace GtkForms */

#endif /* NOTBLANKVALIDATOR_H_ */
