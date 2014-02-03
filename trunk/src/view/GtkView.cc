/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <gtk/gtk.h>
#include "GtkView.h"
#include "Logging.h"
#include <Tiliae.h>
#include "App.h"
#include "Context.h"

namespace GtkForms {
src::logger_mt& lg = logger::get ();

/*
 * Helper class for passing required data between user code and GObject closures.
 */
struct ClosureDTO {
        std::string handlerCopy;
        Context *context = 0;
};

struct GtkView::Impl {
        GtkWidget *widget = 0;
        GtkBuilder *builder = 0;
//        static ClosureDTO closureDTO;

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

//ClosureDTO GtkView::Impl::closureDTO;

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

void GtkView::load (Context *context)
{
        if (!uiFile) {
                throw Core::Exception ("No UiFile object set inside GtkView.");
        }

        impl->builder = uiFile->load ();
        impl->widget = GTK_WIDGET (gtk_builder_get_object (impl->builder, name.c_str ()));

        if (!impl->widget) {
                throw Core::Exception ("No widget with name : [" + name + "] was found in file : [" + uiFile->getFile () + "].");
        }

        connectSignals (context);
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

/*--------------------------------------------------------------------------*/

void GtkView::connectSignals (Context *context)
{
        // https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-connect-signals-full
        gtk_builder_connect_signals_full (impl->builder, GtkView::Impl::myConnectFunc, static_cast <gpointer> (context));

//        window = GTK_WIDGET (gtk_builder_get_object (builder, widgetName.c_str ()));
//        g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), &window);
}

/*--------------------------------------------------------------------------*/

void GtkView::Impl::handler (const std::string &sourceCode, const Core::VariantVector &paramVector, Context *context)
{
        Core::Variant domain {context};
        k202::K202 &k202 = App::getK202 ();

        Core::Variant result = k202.run (sourceCode, domain, paramVector);
        BOOST_LOG (lg) << "handler here : [" << sourceCode << "]. The result : [" << result.toString () << "]";

//        Core::Variant K202::run (const std::string &sourceCode,
//                const Core::Variant &domain,
//                const Core::VariantVector &paramVector,
//                const Core::VariantMap &argsMap)
}

/*--------------------------------------------------------------------------*/

void GtkView::Impl::myConnectFunc (GtkBuilder *builder,
                                   GObject *object,
                                   const gchar *signal_name,
                                   const gchar *handler_name,
                                   GObject *connect_object,
                                   GConnectFlags flags,
                                   gpointer user_data)
{
        BOOST_LOG (lg) << "Connecting : " << signal_name << ", " << handler_name /*<< ", DTO : [" << (void *)&GtkView::Impl::closureDTO << "]"*/;

        // Skopiować napis z signal_name
        ClosureDTO *dto = new ClosureDTO;
//        GtkView::Impl::closureDTO.handlerCopy = g_strdup (handler_name);
//        GtkView::Impl::closureDTO.context = static_cast <Context *> (user_data);
        dto->handlerCopy = /*g_strdup (*/handler_name/*)*/;
        dto->context = static_cast <Context *> (user_data);

        /*
         * https://developer.gnome.org/gobject/stable/gobject-Closures.html#g-cclosure-new
         */
        GClosure *closure = g_cclosure_new (G_CALLBACK (GtkView::Impl::handler), dto, gclosureUserDataDelete);

        // Usunąć napis z signal_name ?
//        g_free (handlerCopy);

        g_closure_set_marshal (closure, gClosureMarshal);
        g_signal_connect_closure  (object, signal_name, closure, FALSE);
}

/*--------------------------------------------------------------------------*/

void GtkView::Impl::gClosureMarshal (GClosure *closure,
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

void GtkView::Impl::gclosureUserDataDelete (gpointer data, GClosure *closure)
{
        ClosureDTO *dto = static_cast <ClosureDTO *> (data);
        delete dto;
}

} // namespace GtkForms
