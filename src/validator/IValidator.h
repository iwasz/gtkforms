/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef IVALIDATOR_H_
#define IVALIDATOR_H_

#include <core/Object.h>
#include <string>
#include "ValidationAndBindingResult.h"
#include <ReflectionParserAnnotation.h>

namespace GtkForms {
class Context;

/**
 *
 */
class __tiliae_reflect__ IValidator : public Core::Object {
public:
        virtual ~IValidator () {}
        virtual ValidationAndBindingResult validate (Context &ctx) const = 0;
        virtual std::string getModel () const = 0;
};

/**
 *
 */
typedef __tiliae_reflect__ std::vector <IValidator *> ValidatorVector;

} /* namespace GtkForms */

#endif /* IVALIDATOR_H_ */
