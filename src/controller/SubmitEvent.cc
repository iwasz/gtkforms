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
#include "view/GtkView.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get();

void SubmitEvent::run (App *app)
{
        BOOST_LOG (lg) << "SubmitEvent::run. viewName : [" << viewName << "], dataRange : [" << dataRange << "], controllerName : [" << controllerName << "].";
        GtkView *v = dynamic_cast <GtkView *> (view);

#if 0
        v->printStructure ();
#endif

        GtkView::InputMap map = v->getInputs (dataRange);

#if 0
        for (auto elem : map) {
                BOOST_LOG (lg) << elem.first;
        }
#endif
}

} // namespace GtkForms



