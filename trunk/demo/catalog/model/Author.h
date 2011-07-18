/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef AUTHOR_H_
#define AUTHOR_H_

#include <Reflection.h>
#include <IToStringEnabled.h>

class Author : public Core::IToStringEnabled {
public:
        __c (void)
        virtual ~Author () {}

        _m (getFirstname) std::string const &getFirstname () const { return firstname; }
        _m (setFirstname) void setFirstname (std::string const &firstname) { this->firstname = firstname; }

        _m (getLastname) std::string const &getLastname () const { return lastname; }
        _m (setLastname) void setLastname (std::string const &lastname) { this->lastname = lastname; }

        std::string toString () const
        {
                return "Author (firstname=" + firstname + ", lastname=" + lastname + ")";
        }

private:

        std::string firstname;
        std::string lastname;

        _e (Author)
};

#	endif /* AUTHOR_H_ */
