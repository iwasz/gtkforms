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
#include <list>

namespace GtkForms {

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
typedef std::list <ValidationAndBindingResult> ValidationResultList;

class ValidationAndBindingResultContainer {
public:

        bool add (ValidationAndBindingResult const &v);
        void clear () { previous = results; results.clear (); }
        ValidationResultList const &getResults () const { return results; }

private:
        ValidationResultList results;
        ValidationResultList previous;
};

}

#endif /* VALIDATIONANDBINDINGRESULT_H_ */
