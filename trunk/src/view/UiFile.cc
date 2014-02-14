/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "UiFile.h"
#include "Context.h"
#include "Logging.h"
#include "App.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get ();

/*
 * Helper class for passing required data between user code and GObject closures.
 * TODO Zostawić łączenie w jednym pliku : albo tu, albo w AbstractGtkView.
 */
struct ClosureDTO {
        std::string handlerCopy;
        Context *context = 0;
};

/**
 *
 */
struct UiFile::Impl {
        GtkBuilder *builder = 0;
        bool loaded = false;
        bool connected = false;

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

/*--------------------------------------------------------------------------*/

UiFile::UiFile ()
{
        impl = new Impl;
}

/*--------------------------------------------------------------------------*/

UiFile::~UiFile ()
{
        destroy ();
        delete impl;
}

/*--------------------------------------------------------------------------*/

GtkBuilder *UiFile::load ()
{
        if (impl->loaded) {
                return impl->builder;
        }

        if (file.empty ()) {
                throw Core::Exception ("You didn't specify a file name to load in this UiFile factory object.");
        }

        GError *err = NULL;

        impl->builder = gtk_builder_new ();
        gtk_builder_add_from_file (impl->builder, file.c_str (), &err);

        if (err) {
                throw Core::Exception ("gtk_builder_add_from_file returned an error. Error message is : [" + std::string (err->message) + "]");
        }

        return impl->builder;
}

/*--------------------------------------------------------------------------*/

void UiFile::destroy ()
{
        g_object_unref (impl->builder);
        impl->loaded = false;
        impl->connected = false;
}

/*--------------------------------------------------------------------------*/

void UiFile::connectSignals (Context *context)
{
        if (impl->connected) {
                return;
        }

        impl->connected = true;
        // https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-connect-signals-full
        gtk_builder_connect_signals_full (impl->builder, UiFile::Impl::myConnectFunc, static_cast <gpointer> (context));

//        window = GTK_WIDGET (gtk_builder_get_object (builder, widgetName.c_str ()));
//        g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), &window);
}

/*--------------------------------------------------------------------------*/

void UiFile::Impl::handler (const std::string &sourceCode, const Core::VariantVector &paramVector, Context *context)
{
        Core::Variant domain {context};
        k202::K202 *k202 = App::getK202 ();

        Core::Variant result = k202->run (sourceCode, domain, paramVector);
        BOOST_LOG (lg) << "handler here : [" << sourceCode << "]. The result : [" << result.toString () << "]";
}

/*--------------------------------------------------------------------------*/

void UiFile::Impl::myConnectFunc (GtkBuilder *builder,
                                   GObject *object,
                                   const gchar *signal_name,
                                   const gchar *handler_name,
                                   GObject *connect_object,
                                   GConnectFlags flags,
                                   gpointer user_data)
{
        BOOST_LOG (lg) << "Connecting : " << signal_name << ", " << handler_name /*<< ", DTO : [" << (void *)&UiFile::Impl::closureDTO << "]"*/;

        // Skopiować napis z signal_name
        ClosureDTO *dto = new ClosureDTO;
//        UiFile::Impl::closureDTO.handlerCopy = g_strdup (handler_name);
//        UiFile::Impl::closureDTO.context = static_cast <Context *> (user_data);
        dto->handlerCopy = /*g_strdup (*/handler_name/*)*/;
        dto->context = static_cast <Context *> (user_data);

        /*
         * https://developer.gnome.org/gobject/stable/gobject-Closures.html#g-cclosure-new
         */
        GClosure *closure = g_cclosure_new (G_CALLBACK (UiFile::Impl::handler), dto, gclosureUserDataDelete);

        // Usunąć napis z signal_name ?
//        g_free (handlerCopy);

        g_closure_set_marshal (closure, gClosureMarshal);
        g_signal_connect_closure  (object, signal_name, closure, FALSE);
}

/*--------------------------------------------------------------------------*/

void UiFile::Impl::gClosureMarshal (GClosure *closure,
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

void UiFile::Impl::gclosureUserDataDelete (gpointer data, GClosure *closure)
{
        ClosureDTO *dto = static_cast <ClosureDTO *> (data);
        delete dto;
}

} /* namespace GtkForms */
