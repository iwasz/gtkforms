/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_IHANDLER_H_
#define GTK_FORMS_IHANDLER_H_

#include <core/Object.h>

namespace GtkForms {
class App;

class __tiliae_reflect__ IHandler : public Core::Object {
public:
        virtual ~IHandler () {}
        virtual void run (App *app) = 0;
};

} /* namespace GtkForms */

#endif /* IHANDLER_H_ */
