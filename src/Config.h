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

namespace GtkForms {

const unsigned int MAIN_LOOP_DEFAULT_DELAY_MS = 10;

struct Config {
        ctr__ (void)
        Config () : loopDelayMs {MAIN_LOOP_DEFAULT_DELAY_MS} {}
        int loopDelayMs;
        end_ (Config)
};

} /* namespace GtkForms */

#endif /* CONFIG_H_ */
