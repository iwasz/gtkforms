/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef GTK_FORMS_REFRESHEVENT_H_
#define GTK_FORMS_REFRESHEVENT_H_

#include "IEvent.h"
#include "view/IView.h"
#include <string>
#include "mapping/Mapping.h"

namespace GtkForms {
class Context;
class IController;

class RefreshEvent : public IEvent {
public:
        virtual ~RefreshEvent () {}
        void run (App *app);

        std::string viewName;
        std::string dataRange;
};

} /* namespace GtkForms */

#endif /* REFRESHEVENT_H_ */
