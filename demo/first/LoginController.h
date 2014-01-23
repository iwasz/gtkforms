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

class LoginController : public AbstractController {
public:
        virtual ~LoginController () {}

        std::string start ();
        std::string onSumit ();
        std::string end ();

private:

        void onScreenKeyboard ();
        void onKeyboardFinish (std::string const &field, std::string const &text);

private:

        LoginService *loginService = 0;
};

#endif /* LOGINCONTROLLER_H_ */
