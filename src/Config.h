/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef APPLICATION_CONFIG_H_
#define APPLICATION_CONFIG_H_

#include <ReflectionParserAnnotation.h>
#include "controller/IHandler.h"

namespace GtkForms {

const unsigned int MAIN_LOOP_DEFAULT_DELAY_MS = 10;

struct __tiliae_reflect__ Config {
        Config () : loopDelayMs {MAIN_LOOP_DEFAULT_DELAY_MS},
                    quitHandler {nullptr},
                    logMappings {false},
                    useWidgetId {false} {}

        /**
         * @brief Events like submit or refresh are cheched every loopDelayMs.
         */
        int loopDelayMs;

        /**
         * @brief As name implies this is a piece of code which gets run when quit was requested.
         */
        IHandler *quitHandler;

        /**
         * @brief Turns on more alaborate debug output about input and ouput widgets, and their mappings.
         */
        bool logMappings;

        /**
         * @brief Tells how GtkWidgets are retrieved from glade *.ui files. If this variable is turned
         * off (I mean false, which is default), gtkforms will get widgets by their "name" property (which
         * is different than "ID", and is available on the "Common" tab under glade). If this variable is
         * true, both "ID" and "name" properties will be considered.
         */
        bool useWidgetId;
};

} /* namespace GtkForms */

#endif /* CONFIG_H_ */
