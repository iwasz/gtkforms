/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef QUITEVENT_H_
#define QUITEVENT_H_

#include "IEvent.h"

class QuitEvent : public IEvent {
public:
        virtual ~QuitEvent ();
        void run (App *app);
};

#endif /* QUITEVENT_H_ */
