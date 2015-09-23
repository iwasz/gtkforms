/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef APPLICATION_CONFIG_H_
#define APPLICATION_CONFIG_H_

#include "ReflectionMacros.h"
#include "controller/IHandler.h"

namespace GtkForms {

const unsigned int MAIN_LOOP_DEFAULT_DELAY_MS = 10;

struct Config {
        REFLECTION_CONSTRUCTOR_ (void)
        Config () : loopDelayMs {MAIN_LOOP_DEFAULT_DELAY_MS},
                    quitHandler {nullptr},
                    logMappings {false} {}
        int REFLECTION_FIELD_VALUE_INPLACE (loopDelayMs);
        IHandler *REFLECTION_FIELD_VALUE_INPLACE (quitHandler);
        bool REFLECTION_FIELD_VALUE_INPLACE (logMappings);
        REFLECTION_END (Config)
};

} /* namespace GtkForms */

#endif /* CONFIG_H_ */
