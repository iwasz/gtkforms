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
#include <GtkForms.h>

namespace GtkForms {

class AbstractValidator : public IValidator {
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

private:

        std::string REFLECTION_FIELD_VALUE_INPLACE (model);
        Core::VariantMap REFLECTION_FIELD_REFERENCE_INPLACE (params);
        REFLECTION_END (AbstractValidator)
};

} /* namespace GtkForms */

#endif /* ABSTRACTVALIDATOR_H_ */
