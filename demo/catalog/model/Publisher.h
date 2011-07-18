/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef PUBLISHER_H_
#define PUBLISHER_H_

#include <Reflection.h>

namespace Catalog {

class Publisher {
public:

        __c (void)
        virtual ~Publisher () {}

        _m (getName) std::string const &getName () const { return name; }
        _m (setName) void setName (std::string const &name) { this->name = name; }

        std::string toString () const { return "Publisher (name=" + name + ")"; }

private:

        std::string name;
        _e (Publisher)
};

}

#	endif /* PUBLISHER_H_ */
