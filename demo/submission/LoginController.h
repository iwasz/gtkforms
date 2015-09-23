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
        REFLECTION_CONSTRUCTOR_ (void)
        REFLECTION_BASE_CLASS ("AbstractController")

        virtual ~LoginController () {}

        std::string start ();
        std::string onSubmit ();
        std::string end ();

        REFLECTION_METHOD (buttonClicked) void buttonClicked (unsigned int column, Core::Variant selectedObject);

private:

        EmployeeVector employees;

        REFLECTION_END (LoginController)
};

#endif /* LOGINCONTROLLER_H_ */
