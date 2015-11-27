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

        std::string onStart () { return view; }
        void onSubmit () {}
        void onStop () {}

public:

        std::string view;
};

} /* namespace GtkForms */

#endif /* DUMMYCONTROLLER_H_ */
