/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "GtkBuilderFactory.h"
#include <gtk/gtk.h>
#include <ostream>

/****************************************************************************/

static void handler (const std::string &sourceCode,
                     const Core::VariantVector &paramVector)
{
        std::cerr << "handler here : " << sourceCode << std::endl;

//        Core::Variant K202::run (const std::string &sourceCode,
//                const Core::Variant &domain,
//                const Core::VariantVector &paramVector,
//                const Core::VariantMap &argsMap)
}

/****************************************************************************/

void gClosureMarshal (GClosure *closure,
                      GValue *return_value,
                      guint n_param_values,
                      const GValue *param_values,
                      gpointer invocation_hint,
                      gpointer marshal_data)
{
        std::cerr << "Marshaler here." << std::endl;

        typedef void (*HandlerType) (const std::string &sourceCode,
                                     const Core::VariantVector &paramVector);

        GCClosure *cc = (GCClosure*) closure;
        HandlerType callback;

        callback = (HandlerType) (marshal_data ? marshal_data : cc->callback);
//        callback = (HandlerType)cc->callback;



        Core::VariantVector params;
        callback (std::string ((gchar *)closure->data), params);
}

/****************************************************************************/

static void myConnectFunc (GtkBuilder *builder,
                           GObject *object,
                           const gchar *signal_name,
                           const gchar *handler_name,
                           GObject *connect_object,
                           GConnectFlags flags,
                           gpointer user_data)
{
        std::cerr << "Connecting : " << signal_name << ", " << handler_name << std::endl;

        // Skopiować napis z signal_name
        gchar *handlerCopy = g_strdup (handler_name);

        // Podać jako parametr tam gdzie user_data
        GClosure *closure = g_cclosure_new (G_CALLBACK (handler), handlerCopy, NULL);

        // Usunąć napis z signal_name ?
//        g_free (handlerCopy);

        g_closure_set_marshal (closure, gClosureMarshal);
        g_signal_connect_closure  (object, signal_name, closure, FALSE);
}

/****************************************************************************/

GtkWidget *GtkBuilderFactory::create (bool show)
{
        static GtkWidget *window = NULL;
        GError *err = NULL;

        if (!window) {
                GtkBuilder *builder = gtkBuilderAdapter->getBuilder ();

                if (!fileName.empty ()) {
                        gtk_builder_add_from_file (builder, fileName.c_str (), &err);
                }

                if (err) {
                        g_error ("ERROR: %s\n", err->message);
                        return NULL;
                }

//                gtk_builder_connect_signals (builder, NULL);
                gtk_builder_connect_signals_full (builder, myConnectFunc, NULL);

                window = GTK_WIDGET (gtk_builder_get_object (builder, widgetName.c_str ()));
                g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), &window);
//                g_signal_connect (window, "destroy", G_CALLBACK (gtk_widget_destroyed), &window);

                if (show) {
                        gtk_widget_show (window);
                }
        }

        if (!gtk_widget_get_visible (window)) {
                gtk_widget_show_all (window);
        }
        else {
                gtk_widget_destroy (window);
                window = NULL;
        }

        return window;
}
