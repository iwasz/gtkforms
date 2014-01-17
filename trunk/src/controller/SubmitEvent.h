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

class SubmitEvent : public IEvent {
public:
        SubmitEvent () {}
        virtual ~SubmitEvent (){}
        void run (App *app);
};

#endif /* SUBMITEVENT_H_ */
