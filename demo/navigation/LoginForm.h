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
        REFLECTION_CONSTRUCTOR_ (void)
        std::string REFLECTION_FIELD_VALUE_INPLACE (login);
        std::string REFLECTION_FIELD_VALUE_INPLACE (password);
        REFLECTION_END (LoginForm)
};

#endif /* LOGINFORM_H_ */
