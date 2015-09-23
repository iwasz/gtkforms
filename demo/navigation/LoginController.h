/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef LOGINCONTROLLER_H_
#define LOGINCONTROLLER_H_

#include <GtkForms.h>
#include "LoginService.h"
#include <string>

class LoginController : public GtkForms::AbstractController {
public:
        REFLECTION_CONSTRUCTOR_ (void)
        REFLECTION_BASE_CLASS ("AbstractController")

        virtual ~LoginController () {}

        std::string start ();
        std::string onSubmit ();
        std::string end ();

private:

        void onScreenKeyboard ();
        void onKeyboardFinish (std::string const &field, std::string const &text);

private:

        LoginService *loginService = 0;

        REFLECTION_END (LoginController)
};

#endif /* LOGINCONTROLLER_H_ */
