/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "SubmitEvent.h"
#include "App.h"
#include "Logging.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get();

void SubmitEvent::run (App *app)
{
        BOOST_LOG (lg) << "SubmitEvent::run. viewName : [" << viewName << "], dataRange : [" << dataRange << "], controllerName : [" << controllerName << "].";
}

} // namespace GtkForms



