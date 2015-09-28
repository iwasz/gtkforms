/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef DUMMYCONTROLLER_H_
#define DUMMYCONTROLLER_H_

#include <string>
#include "AbstractController.h"

namespace GtkForms {

class __tiliae_reflect__ DummyController : public AbstractController {
public:
        REFLECTION_CONSTRUCTOR_ (void)
        REFLECTION_BASE_CLASS ("AbstractController")

        virtual ~DummyController () {}

        std::string start () { return viewCommand; }
        std::string onSubmit () { return ""; }
        std::string end () { return ""; }

private:

        std::string REFLECTION_FIELD_REFERENCE_INPLACE (viewCommand);
        REFLECTION_END (DummyController)
};

} /* namespace GtkForms */

#endif /* DUMMYCONTROLLER_H_ */
