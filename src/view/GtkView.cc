/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <gtk/gtk.h>
#include <Tiliae.h>
#include "GtkView.h"
#include "Logging.h"
#include "App.h"
#include "Context.h"
#include "Slot.h"

namespace GtkForms {
using namespace std;
static src::logger_mt &lg = logger::get ();

/**
 *
 */
struct GtkView::Impl {
        static void onUserClickedQuit (GtkWidget *object, gpointer userData);
};

/****************************************************************************/

GtkView::GtkView () { impl = new Impl; }

/*--------------------------------------------------------------------------*/

GtkView::~GtkView () { delete impl; }

/*--------------------------------------------------------------------------*/

void GtkView::loadUi (App *app)
{
        AbstractView::loadUi (app);

        if (!GTK_IS_WINDOW (getUi ())) {
                throw Core::Exception ("GtkView::loadUi : failed to cast ui object to GtkWindow.");
        }

        g_signal_connect (GTK_WINDOW (getUi ()), "destroy", G_CALLBACK (&GtkView::Impl::onUserClickedQuit), this);
}

/*--------------------------------------------------------------------------*/

void GtkView::show () { gtk_widget_show (GTK_WIDGET (getUi ())); }

/*--------------------------------------------------------------------------*/

void GtkView::Impl::onUserClickedQuit (GtkWidget *object, gpointer userData)
{
        GtkView *view = static_cast<GtkView *> (userData);
        AbstractController *controller = view->getController ();
        App *app = controller->getApp ();

        // Close this controller
        if (controller) {
                app->close (controller);
        }
        else {
                app->userQuitRequest ();
        }
}

} // namespace GtkForms
