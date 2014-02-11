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

class DummyController : public AbstractController {
public:
        ctr__ (void)
        bse_ ("AbstractController")

        virtual ~DummyController () {}

        std::string start () { return viewCommand; }
        std::string onSubmit () { return ""; }
        std::string end () { return ""; }

private:

        std::string prr_ (viewCommand);
        end_ (DummyController)
};

} /* namespace GtkForms */

#endif /* DUMMYCONTROLLER_H_ */
