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
        ctr__ (void)
        bse_ ("AbstractValidator")
        virtual ~NotBlankValidator () {}
        virtual ValidationAndBindingResult validate (Context &ctx) const;
        end_ (NotBlankValidator)
};

} /* namespace GtkForms */

#endif /* NOTBLANKVALIDATOR_H_ */
