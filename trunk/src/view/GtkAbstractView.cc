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

namespace GtkForms {
static src::logger_mt& lg = logger::get ();

/*
 * Helper class for passing required data between user code and GObject closures.
 */
struct ClosureDTO {
        std::string handlerCopy;
        Context *context = 0;
};

struct GtkAbstractView::Impl {
        GtkWidget *widget = 0;
        GtkBuilder *builder = 0;

        static void handler (const std::string &sourceCode, const Core::VariantVector &paramVector, Context *context);

        /*
         * https://developer.gnome.org/gtk3/stable/GtkBuilder.html#GtkBuilderConnectFunc
         */
        static void myConnectFunc (GtkBuilder *builder,
                                   GObject *object,
                                   const gchar *signal_name,
                                   const gchar *handler_name,
                                   GObject *connect_object,
                                   GConnectFlags flags,
                                   gpointer user_data);

        static void gClosureMarshal (GClosure *closure,
                                     GValue *return_value,
                                     guint n_param_values,
                                     const GValue *param_values,
                                     gpointer invocation_hint,
                                     gpointer marshal_data);

        static void gclosureUserDataDelete (gpointer data, GClosure *closure);

};

//ClosureDTO GtkAbstractView::Impl::closureDTO;

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

void GtkAbstractView::loadUi (Context *context)
{
        if (!uiFile) {
                throw Core::Exception ("No UiFile object set inside GtkAbstractView.");
        }

        if (impl->widget) {
                return;
        }

        impl->builder = uiFile->load ();
        impl->widget = GTK_WIDGET (gtk_builder_get_object (impl->builder, name.c_str ()));

        if (!impl->widget) {
                throw Core::Exception ("No widget with name : [" + name + "] was found in file : [" + uiFile->getFile () + "].");
        }

//        uiFile->connectSignals (context);
        connectSignals (context);
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

void GtkAbstractView::connectSignals (Context *context)
{
        // https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-connect-signals-full
        gtk_builder_connect_signals_full (impl->builder, GtkAbstractView::Impl::myConnectFunc, static_cast <gpointer> (context));

//        window = GTK_WIDGET (gtk_builder_get_object (builder, widgetName.c_str ()));
//        g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), &window);
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::Impl::handler (const std::string &sourceCode, const Core::VariantVector &paramVector, Context *context)
{
        Core::Variant domain {context};
        k202::K202 &k202 = App::getK202 ();

        Core::Variant result = k202.run (sourceCode, domain, paramVector);
        BOOST_LOG (lg) << "handler here : [" << sourceCode << "]. The result : [" << result.toString () << "]";
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::Impl::myConnectFunc (GtkBuilder *builder,
                                   GObject *object,
                                   const gchar *signal_name,
                                   const gchar *handler_name,
                                   GObject *connect_object,
                                   GConnectFlags flags,
                                   gpointer user_data)
{
        BOOST_LOG (lg) << "Connecting : " << signal_name << ", " << handler_name /*<< ", DTO : [" << (void *)&GtkAbstractView::Impl::closureDTO << "]"*/;

        // Skopiować napis z signal_name
        ClosureDTO *dto = new ClosureDTO;
//        GtkAbstractView::Impl::closureDTO.handlerCopy = g_strdup (handler_name);
//        GtkAbstractView::Impl::closureDTO.context = static_cast <Context *> (user_data);
        dto->handlerCopy = /*g_strdup (*/handler_name/*)*/;
        dto->context = static_cast <Context *> (user_data);

        /*
         * https://developer.gnome.org/gobject/stable/gobject-Closures.html#g-cclosure-new
         */
        GClosure *closure = g_cclosure_new (G_CALLBACK (GtkAbstractView::Impl::handler), dto, gclosureUserDataDelete);

        // Usunąć napis z signal_name ?
//        g_free (handlerCopy);

        g_closure_set_marshal (closure, gClosureMarshal);
        g_signal_connect_closure  (object, signal_name, closure, FALSE);
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::Impl::gClosureMarshal (GClosure *closure,
                                     GValue *return_value,
                                     guint n_param_values,
                                     const GValue *param_values,
                                     gpointer invocation_hint,
                                     gpointer marshal_data)
{
        BOOST_LOG (lg) << "Marshaler here. closure : [" << (void *)closure << "], n_param_values : [" << n_param_values << "], invocation_hint : ["
                       << (void *)invocation_hint << "], marshal_data [" << (void *)marshal_data << "]";

        typedef void (*HandlerType) (const std::string &,
                                     const Core::VariantVector &,
                                     Context *);

        GCClosure *cc = (GCClosure*) closure;
        HandlerType callback = (HandlerType) (marshal_data ? marshal_data : cc->callback);

        Core::VariantVector params;
        ClosureDTO *dto = static_cast <ClosureDTO *> (closure->data);
        callback (/*std::string (*/dto->handlerCopy/*)*/, params, dto->context);
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::Impl::gclosureUserDataDelete (gpointer data, GClosure *closure)
{
        ClosureDTO *dto = static_cast <ClosureDTO *> (data);
        delete dto;
}

} // namespace GtkForms
