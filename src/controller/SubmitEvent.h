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
class Context;
class AbstractController;

struct SubmitEvent : public IEvent {

        virtual ~SubmitEvent (){}
        void run (App *app);

        AbstractController *controller = nullptr;
        std::string controllerName;
        std::string inputRange;
};

} // namespace GtkForms

#endif /* SUBMITEVENT_H_ */
