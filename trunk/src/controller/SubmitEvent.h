/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef SUBMITEVENT_H_
#define SUBMITEVENT_H_

#include "IEvent.h"
#include <string>

namespace GtkForms {
class IView;

struct SubmitEvent : public IEvent {

        virtual ~SubmitEvent (){}
        void run (App *app);

        std::string viewName;
        std::string dataRange;
        std::string controllerName;
        IView *view = 0;
};

} // namespace GtkForms

#endif /* SUBMITEVENT_H_ */
