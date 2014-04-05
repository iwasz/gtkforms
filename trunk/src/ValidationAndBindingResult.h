/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef VALIDATIONANDBINDINGRESULT_H_
#define VALIDATIONANDBINDINGRESULT_H_

#include <string>
#include <Tiliae.h>
#include <vector>

/**
 *
 */
struct ValidationAndBindingResult {
        ValidationAndBindingResult () {}

        void addParam (std::string const &key, Core::Variant value) { params[key] = value; }

        std::string model;
        bool valid = true;
        Core::VariantMap params;
};

/**
 *
 */
typedef std::vector <ValidationAndBindingResult> ValidationResultVector;

#endif /* VALIDATIONANDBINDINGRESULT_H_ */
