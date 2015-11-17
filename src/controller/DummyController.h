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

        virtual ~DummyController () {}

        std::string start () { return viewCommand; }
        std::string onSubmit () { return ""; }
        std::string end () { return ""; }

public:

        std::string viewCommand;
};

} /* namespace GtkForms */

#endif /* DUMMYCONTROLLER_H_ */
