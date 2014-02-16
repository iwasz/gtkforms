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
#include "mapping/Mapping.h"

namespace GtkForms {
class IView;
class Context;
class IController;

struct SubmitEvent : public IEvent {

        virtual ~SubmitEvent (){}
        void run (App *app);

        std::string viewName;
        std::string controllerName;
        std::string dataRange;
};

} // namespace GtkForms

#endif /* SUBMITEVENT_H_ */
