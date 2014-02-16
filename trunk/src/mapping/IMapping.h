/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef IMAPPING_H_
#define IMAPPING_H_

#include <string>
#include <core/Object.h>

namespace GtkForms {

class IMapping : public Core::Object {
public:
        virtual ~IMapping () {}

        virtual std::string getInput () const = 0;
        virtual std::string getProperty () const = 0;
        virtual std::string getModel () const = 0;
};

} /* namespace GtkForms */

#endif /* IMAPPING_H_ */
