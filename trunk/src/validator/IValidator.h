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
#include <vector>
#include "ReflectionMacros.h"

namespace GtkForms {
class Context;

/**
 *
 */
struct ValidationResult {
        ValidationResult (std::string const &m) : message {m} {}
        ValidationResult () {}

        std::string model;
        std::string message;
        bool valid = true;

};

/**
 *
 */
typedef std::vector <ValidationResult> ValidationResultVector;

/**
 *
 */
class IValidator : public Core::Object {
public:
        virtual ~IValidator () {}
        virtual ValidationResult validate (Context &ctx) const = 0;
        virtual std::string getModel () const = 0;
};

/**
 *
 */
typedef std::vector <IValidator *> ValidatorVector;
col_ (ValidatorVector)

} /* namespace GtkForms */

#endif /* IVALIDATOR_H_ */
