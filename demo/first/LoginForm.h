/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef LOGINFORM_H_
#define LOGINFORM_H_

#include <Tiliae.h>
#include <ReflectionMacros.h>

struct LoginForm {
        ctr__ (void)
        std::string prp_ (login);
        std::string prp_ (password);
        end_ (LoginForm)
};

#endif /* LOGINFORM_H_ */
