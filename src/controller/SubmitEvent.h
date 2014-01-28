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

namespace GtkForms {

class SubmitEvent : public IEvent {
public:
        SubmitEvent () {}
        virtual ~SubmitEvent (){}
        void run (App *app);
};

} // namespace GtkForms

#endif /* SUBMITEVENT_H_ */
