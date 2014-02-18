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
#include <string>
#include "Employee.h"

class LoginController : public GtkForms::AbstractController {
public:
        ctr__ (void)
        bse_ ("AbstractController")

        virtual ~LoginController () {}

        std::string start ();
        std::string onSubmit ();
        std::string end ();

        mth_ (buttonClicked) void buttonClicked ();

private:

        EmployeeVector employees;

        end_ (LoginController)
};

#endif /* LOGINCONTROLLER_H_ */
