/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <gtk/gtk.h>
#include "view/AbstractView.h"
#include "ValidationDecorator.h"
#include "Logging.h"
#include "Context.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get();

void ValidationDecorator::run (AbstractView *view, Context *ctx)
{
        if (!ctx->getValidationResults ()) {
                return;
        }

        ValidationResultList const &results = ctx->getValidationResults ()->getResults ();

        if (results.empty ()) {
                return;
        }

        for (ValidationAndBindingResult const &result : results) {
                AbstractView::InputMap inputMap = view->getInputs (result.model);

                AbstractView::InputMap::const_iterator i;
                if ((i = inputMap.find (result.model)) != inputMap.end ()) {
#if 0
                        BOOST_LOG (lg) << result.model;
#endif
                        GtkWidget *widget = i->second;

                        if (!result.valid) {
                                GdkRGBA color = {1, 0, 0, 0.2};
                                gtk_widget_override_background_color (widget, GTK_STATE_FLAG_NORMAL, &color);
                        }
                        else {
                                GdkRGBA color = {1, 1, 1, 1};
                                gtk_widget_override_background_color (widget, GTK_STATE_FLAG_NORMAL, &color);
                        }
                }
        }

}

} /* namespace GtkForms */
