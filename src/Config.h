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
                    logMappings {false} {}

        int loopDelayMs;
        IHandler *quitHandler;
        bool logMappings;
};

} /* namespace GtkForms */

#endif /* CONFIG_H_ */
