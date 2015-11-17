/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ABSTRACTVALIDATOR_H_
#define ABSTRACTVALIDATOR_H_

#include "IValidator.h"
#include <ReflectionParserAnnotation.h>

namespace GtkForms {

class __tiliae_reflect__ AbstractValidator : public IValidator {
public:
        REFLECTION_CLASS
        virtual ~AbstractValidator () {}
        virtual std::string getModel () const { return model; }

protected:

        virtual ValidationAndBindingResult createResult (bool valid = true) const
        {
                ValidationAndBindingResult vr;
                vr.params = params;
                vr.valid = valid;
                vr.model = model;
                return vr;
        }

public:

        std::string model;
        Core::VariantMap params;
};

} /* namespace GtkForms */

#endif /* ABSTRACTVALIDATOR_H_ */
