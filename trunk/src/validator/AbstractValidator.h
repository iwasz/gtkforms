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

        virtual ValidationResult createResult (bool valid = true, std::string const &m = "") const
        {
                ValidationResult vr;
                vr.message = m;
                vr.valid = valid;
                vr.model = model;
                return vr;
        }

private:

        std::string prp_ (model);
        end_ (AbstractValidator)
};

} /* namespace GtkForms */

#endif /* ABSTRACTVALIDATOR_H_ */
