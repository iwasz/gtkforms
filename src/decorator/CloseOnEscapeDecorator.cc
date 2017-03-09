/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "CloseOnEscapeDecorator.h"
#include "controller/AbstractController.h"
#include "view/AbstractView.h"

namespace GtkForms {

void CloseOnEscapeDecorator::preShow (AbstractView *view, Context *ctx)
{
        GtkWidget *widget GTK_WIDGET (view->getUi ());
        g_signal_connect (widget, "key-press-event", G_CALLBACK (&CloseOnEscapeDecorator::onKeyPress), view);
}

/*****************************************************************************/

gboolean CloseOnEscapeDecorator::onKeyPress (GtkWidget *widget, GdkEvent *event, gpointer userData)
{
        if (event->key.keyval == GDK_KEY_Escape && !event->key.is_modifier) {
                AbstractView *view = static_cast<AbstractView *> (userData);
                AbstractController *controller = view->getController ();
                controller->closeThis ();
        }

        return false;
}

} // namespace
