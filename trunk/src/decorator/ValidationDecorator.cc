/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "ValidationDecorator.h"
#include "Logging.h"
#include "Context.h"
#include "Page.h"
#include <gtk/gtk.h>

namespace GtkForms {
static src::logger_mt& lg = logger::get();

void ValidationDecorator::run (Page *page, Context *ctx)
{
        ValidationResultVector &results = ctx->getValidationResults ();

        if (results.empty ()) {
                return;
        }

        GtkView *view = page->getView ();

        for (ValidationAndBindingResult const &result : results) {
                GtkView::InputMap inputMap = view->getInputs (result.model);

                GtkView::InputMap::const_iterator i;
                if ((i = inputMap.find (result.model)) != inputMap.end ()) {
                        BOOST_LOG (lg) << result.model;
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
