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
};

extern "C" {

/*
 * Helper class for passing required data between user code and GObject closures.
 */
struct ClosureDTO {
        char *signalName;
        char *gObjectName;
        char *handler;
        Context *context;
};

/*
 * https://developer.gnome.org/gtk3/stable/GtkBuilder.html#GtkBuilderConnectFunc
 */
void myConnectFunc (GtkBuilder *builder,
                           GObject *object,
                           const gchar *signal_name,
                           const gchar *handler_name,
                           GObject *connect_object,
                           GConnectFlags flags,
                           gpointer user_data);

void gClosureMarshal (GClosure *closure,
                             GValue *return_value,
                             guint n_param_values,
                             const GValue *param_values,
                             gpointer invocation_hint,
                             gpointer marshal_data);

void gclosureUserDataDelete (gpointer data, GClosure *closure);

void handler (const std::string &sourceCode, const Core::VariantVector &paramVector, Context *context);
}

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
        gtk_builder_connect_signals_full (impl->builder, myConnectFunc, static_cast <gpointer> (context));

//        window = GTK_WIDGET (gtk_builder_get_object (builder, widgetName.c_str ()));
//        g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), &window);
}

/*--------------------------------------------------------------------------*/

void handler (const std::string &sourceCode, const Core::VariantVector &paramVector, Context *context)
{
        Core::Variant domain {context};
        k202::K202 *k202 = App::getK202 ();

        Core::Variant result = k202->run (sourceCode, domain, paramVector);
        BOOST_LOG (lg) << "handler here : [" << sourceCode << "]. The result : [" << result.toString () << "]";
}

/*--------------------------------------------------------------------------*/

void myConnectFunc (GtkBuilder *builder,
                    GObject *object,
                    const gchar *signal_name,
                    const gchar *handler_name,
                    GObject *connect_object,
                    GConnectFlags flags,
                    gpointer user_data)
{
        BOOST_LOG (lg) << "Connecting : " << signal_name << ", " << handler_name /*<< ", DTO : [" << (void *)&GtkAbstractView::Impl::closureDTO << "]"*/;

        // Skopiować napis z signal_name
        ClosureDTO *dto = static_cast <ClosureDTO *> (malloc (sizeof (ClosureDTO)));

        char const *tmpString = handler_name;
        tmpString = ((tmpString) ? (tmpString) : (""));
        dto->handler = g_strdup (tmpString);

        dto->context = static_cast <Context *> (user_data);

        tmpString = G_OBJECT_TYPE_NAME (object);
        tmpString = ((tmpString) ? (tmpString) : (""));
        dto->gObjectName = g_strdup (tmpString);

        tmpString = signal_name;
        tmpString = ((tmpString) ? (tmpString) : (""));
        dto->signalName = g_strdup (tmpString);

        /*
         * https://developer.gnome.org/gobject/stable/gobject-Closures.html#g-cclosure-new
         */
        GClosure *closure = g_cclosure_new (G_CALLBACK (handler), dto, gclosureUserDataDelete);

        g_closure_set_marshal (closure, gClosureMarshal);
        g_signal_connect_closure  (object, signal_name, closure, FALSE);
}

/*--------------------------------------------------------------------------*/

void gClosureMarshal (GClosure *closure,
                                     GValue *return_value,
                                     guint n_param_values,
                                     const GValue *param_values,
                                     gpointer invocation_hint,
                                     gpointer marshal_data)
{
        BOOST_LOG (lg) << "Marshaler here. closure : [" << (void *)closure << "], n_param_values : [" << n_param_values << "], invocation_hint : ["
                       << (void *)invocation_hint << "], marshal_data [" << (void *)marshal_data << "]";

        ClosureDTO *dto = static_cast <ClosureDTO *> (closure->data);

        std::string signalNameCopy = dto->signalName;
        std::string handlerCopy = dto->handler;
        std::string gObjectNameCopy = dto->gObjectName;

        if (signalNameCopy == "row-activated" && gObjectNameCopy == "GtkTreeView") {
                // Debug
                if (G_VALUE_TYPE (param_values + 1) == GTK_TYPE_TREE_PATH) {
                        BOOST_LOG (lg) << "GTK_TREE_PATH, " << g_type_name (G_TYPE_FUNDAMENTAL(G_VALUE_TYPE (param_values + 1)));
                }

                if (G_VALUE_TYPE (param_values + 2) == GTK_TYPE_TREE_VIEW_COLUMN) {
                        BOOST_LOG (lg) << "GTK_TYPE_TREE_VIEW_COLUMN, " << g_type_name (G_TYPE_FUNDAMENTAL(G_VALUE_TYPE (param_values + 1)));;
                }

                for (unsigned int i = 0; i < n_param_values; ++i) {
                        BOOST_LOG (lg) << "Parameter : " << G_VALUE_TYPE_NAME (param_values + i);
                }

                // Actual work:
                GValue const *pathVal = param_values + 1;
                if (!G_VALUE_HOLDS_BOXED (pathVal)) {
                        throw Core::Exception ("GValue not boxed.");
                }

                gpointer pathPtr = g_value_get_boxed (pathVal);

//                if (!GTK_IS_TREE_PATH (pathPtr)) {
//                        throw Core::Exception ("");
//                }

                GtkTreePath *path = GTK_TREE_PATH (pathPtr);

        }

        typedef void (*HandlerType) (const std::string &,
                                     const Core::VariantVector &,
                                     Context *);

        GCClosure *cc = (GCClosure*) closure;
        HandlerType callback = (HandlerType) (marshal_data ? marshal_data : cc->callback);

        Core::VariantVector params;
        callback (dto->handler, params, dto->context);
}

/*--------------------------------------------------------------------------*/

void gclosureUserDataDelete (gpointer data, GClosure *closure)
{
        ClosureDTO *dto = static_cast <ClosureDTO *> (data);
        g_free (dto->gObjectName);
        g_free (dto->handler);
        g_free (dto->signalName);
        free (dto);
}

} // namespace GtkForms
