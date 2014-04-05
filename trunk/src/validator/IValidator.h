/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef IVALIDATOR_H_
#define IVALIDATOR_H_

#include <Tiliae.h>
#include <string>
#include "ReflectionMacros.h"
#include "ValidationAndBindingResult.h"

namespace GtkForms {
class Context;

/**
 *
 */
class IValidator : public Core::Object {
public:
        virtual ~IValidator () {}
        virtual ValidationAndBindingResult validate (Context &ctx) const = 0;
        virtual std::string getModel () const = 0;
};

/**
 *
 */
typedef std::vector <IValidator *> ValidatorVector;
col_ (ValidatorVector)

} /* namespace GtkForms */

#endif /* IVALIDATOR_H_ */
