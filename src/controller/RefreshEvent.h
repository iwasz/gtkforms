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
#include <string>

namespace GtkForms {

class App;
class AbstractController;

/**
 * @brief The RefreshEvent class
 */
class RefreshEvent : public IEvent {
public:
        virtual ~RefreshEvent () {}
        void run (App *app);

        AbstractController *controller = nullptr;

        /**
         * Models to be converted and displayed on the view. If empty, then all inputs wil be
         * iterated and models set according to mappings.
         */
        std::string widgetNameRange;

        /// Set only this property
        std::string propertyName;
};

} /* namespace GtkForms */

#endif /* REFRESHEVENT_H_ */
