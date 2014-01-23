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
        C__ (void)
        std::string P_ (login);
        std::string P_ (password);
        E_ (LoginForm)
};

#endif /* LOGINFORM_H_ */
