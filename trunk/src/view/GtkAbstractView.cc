/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <gtk/gtk.h>
#include "GtkAbstractView.h"
#include <Tiliae.h>
#include "App.h"
#include "Context.h"
#include "Logging.h"
#include "mapping/GValueVariant.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get ();

struct GtkAbstractView::Impl {
        GtkWidget *widget = 0;
        GtkBuilder *builder = 0;
//        ConnectDTO connectDTO;
};

/*--------------------------------------------------------------------------*/

GtkAbstractView::GtkAbstractView ()
{
        impl = new Impl;
}

/*--------------------------------------------------------------------------*/

GtkAbstractView::~GtkAbstractView ()
{
        delete impl;
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::loadUi (App *app)
{
        if (!uiFile) {
                throw Core::Exception ("No UiFile object set inside GtkAbstractView.");
        }

        if (impl->widget) {
                return;
        }

        impl->builder = uiFile->load (app);
        impl->widget = GTK_WIDGET (gtk_builder_get_object (impl->builder, name.c_str ()));

        if (!impl->widget) {
                throw Core::Exception ("No widget with name : [" + name + "] was found in file : [" + uiFile->getFile () + "].");
        }
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::show ()
{
        gtk_widget_show (impl->widget);
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::hide ()
{
        gtk_widget_hide (impl->widget);
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::destroyUi ()
{
        hide ();
        gtk_widget_destroy (impl->widget);
        impl->widget = 0;
        impl->builder = 0;
}

/*--------------------------------------------------------------------------*/

GObject *GtkAbstractView::getUi ()
{
        return G_OBJECT (impl->widget);
}

/*--------------------------------------------------------------------------*/

GObject *GtkAbstractView::getUi (std::string const &name)
{
        GObject *obj =  gtk_builder_get_object (impl->builder, name.c_str ());

        if (!obj) {
                throw Core::Exception ("GtkAbstractView::getGObject could not find ARBITRARY object in UI. Ui file : [" + uiFile->getFile () + "], object name : [" + std::string (name) + "].");
        }

        return obj;

#if 0
        GList *children, *iter;

        children = gtk_container_get_children(GTK_CONTAINER(container));
        for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
        g_list_free(children);

        // -----

        if(GTK_IS_CONTAINER(widget)) {
                GList *children = gtk_container_get_children(GTK_CONTAINER(widget));
                ...
        }
        If the widget is a GtkBin it has only one child. In that case, the following is simpler than dealing with a GList:

        if(GTK_IS_BIN(widget)) {
                GtkWidget *child = gtk_bin_get_child(GTK_BIN(widget));
                ...
        }

#endif
}

/*--------------------------------------------------------------------------*/

} // namespace GtkForms
