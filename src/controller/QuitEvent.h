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

namespace GtkForms {

class QuitEvent : public IEvent {
public:
        virtual ~QuitEvent ();
        void run (App *app);
};

} // namespace GtkForms

#endif /* QUITEVENT_H_ */
