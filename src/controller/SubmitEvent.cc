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
#include "Context.h"
#include "Controller.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get();
using namespace Wrapper;
using namespace Core;

void SubmitEvent::run (App *app)
{
        app->doSubmit (this);

}

} // namespace GtkForms



