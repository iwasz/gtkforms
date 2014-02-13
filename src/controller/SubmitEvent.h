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

class SubmitEvent : public IEvent {
public:
        SubmitEvent (std::string const &v,
                     std::string const &d,
                     std::string const &c) :
                     viewName {v}, dataRange {d}, controllerName {c}  {}

        virtual ~SubmitEvent (){}
        void run (App *app);

private:

        std::string viewName;
        std::string dataRange;
        std::string controllerName;
};

} // namespace GtkForms

#endif /* SUBMITEVENT_H_ */
