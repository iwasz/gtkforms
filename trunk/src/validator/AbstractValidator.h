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
        abt__
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

        std::string prp_ (model);
        Core::VariantMap prr_ (params);
        end_ (AbstractValidator)
};

} /* namespace GtkForms */

#endif /* ABSTRACTVALIDATOR_H_ */
