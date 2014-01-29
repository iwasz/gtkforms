/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <gtk/gtk.h>
#include "GtkView.h"

namespace GtkForms {

struct GtkView::Impl {
        GtkWidget *widget = 0;
        GtkBuilder *builder = 0;
};

/*--------------------------------------------------------------------------*/

GtkView::GtkView ()
{
        impl = new Impl;
}

/*--------------------------------------------------------------------------*/

GtkView::~GtkView ()
{
        delete impl;
}

/*--------------------------------------------------------------------------*/

void GtkView::load ()
{
        if (!uiFile) {
                throw Core::Exception ("No UiFile object set inside GtkView.");
        }

        impl->builder = uiFile->load ();
        impl->widget = GTK_WIDGET (gtk_builder_get_object (impl->builder, name.c_str ()));

        if (!impl->widget) {
                throw Core::Exception ("No widget with name : [" + name + "] was found in file : [" + uiFile->getFile () + "].");
        }
}

/*--------------------------------------------------------------------------*/

void GtkView::show ()
{
//        gtk_builder_connect_signals (builder, NULL);
//        gtk_builder_connect_signals_full (builder, myConnectFunc, NULL);


//                TODO Podłączyć do czegoś, co będzie wysylać QuitEvent do wszystkich kontrolerów.
//                g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), &window);
//                g_signal_connect (window, "destroy", G_CALLBACK (gtk_widget_destroyed), &window);

        gtk_widget_show (impl->widget);
}

/*--------------------------------------------------------------------------*/

void GtkView::hide ()
{

}

/*--------------------------------------------------------------------------*/

void GtkView::destroy ()
{
        // TODO : if (toplevel)
        gtk_widget_destroy (impl->widget);
        impl->widget = 0;
        impl->builder = 0;
}

/*--------------------------------------------------------------------------*/

void GtkView::model2View (std::string const &dataRange)
{

}

/*--------------------------------------------------------------------------*/

void GtkView::view2Model (std::string const &dataRange)
{

}

/*--------------------------------------------------------------------------*/

GObject *GtkView::getGObject (std::string const &name)
{
        GObject *obj =  gtk_builder_get_object (impl->builder, name.c_str ());

        if (!obj) {
                throw Core::Exception ("GtkView::getGObject could not find object in UI. Ui file : [" + uiFile->getFile () + "], object name : [" + std::string (name) + "].");
        }

        return obj;
}

} // namespace GtkForms
