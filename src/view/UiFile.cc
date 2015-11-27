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
#include "mapping/GValueVariant.h"
#include "signalAdapter/AbstractSignalAdapter.h"
#include "controller/AbstractAccessor.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get ();

extern "C" {
/*
 * Helper class for passing required data between user code and GObject closures.
 */
struct ClosureDTO {
        char *signal;
        char *gObjectName;
        char const *widgetId;
        char *handler;
        AbstractAccessor *accessor;
};

struct ConnectDTO {
        AbstractAccessor *accessor;
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

void handler (const std::string &sourceCode, const Core::VariantVector &paramVector, AbstractAccessor *accessor);

} // extern "C"

/**
 *
 */
struct UiFile::Impl {
        GtkBuilder *builder = 0;
        bool loaded = false;
        ConnectDTO connectDTO;
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

GtkBuilder *UiFile::getBuilder ()
{
        return impl->builder;
}

/*--------------------------------------------------------------------------*/

GtkBuilder *UiFile::load (AbstractAccessor *accessor)
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

        connectSignals (accessor);

        impl->loaded = true;
        return impl->builder;
}

/*--------------------------------------------------------------------------*/

void UiFile::destroy ()
{
        g_object_unref (impl->builder);
        impl->loaded = false;
        impl->builder = nullptr;
}

/*--------------------------------------------------------------------------*/

void UiFile::connectSignals (AbstractAccessor *accessor)
{
        impl->connectDTO.accessor = accessor;
        // https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-connect-signals-full
        gtk_builder_connect_signals_full (impl->builder, myConnectFunc, static_cast <gpointer> (&impl->connectDTO));

        BOOST_LOG (lg) << "builder = [" << impl->builder << "], myConnectFunc = [" << myConnectFunc << "], connectDTO = [" << &impl->connectDTO << "]";
}

/*--------------------------------------------------------------------------*/

void handler (const std::string &sourceCode, const Core::VariantVector &paramVector, AbstractAccessor *accessor)
{
        Core::Variant domain {accessor};
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
        BOOST_LOG (lg) << "Connecting : " << signal_name << ", " << handler_name /*<< ", DTO : [" << (void *)&UiFile::Impl::closureDTO << "]"*/;

        // Skopiować napis z signal_name
        ClosureDTO *closureDTO = static_cast <ClosureDTO *> (malloc (sizeof (ClosureDTO)));

        char const *tmpString = handler_name;
        tmpString = ((tmpString) ? (tmpString) : (""));
        closureDTO->handler = g_strdup (tmpString);

        ConnectDTO *connectDTO = static_cast <ConnectDTO *> (user_data);
//        closureDTO->context = &connectDTO->app->getContext ();
        closureDTO->accessor = connectDTO->accessor;

        tmpString = G_OBJECT_TYPE_NAME (object);
        tmpString = ((tmpString) ? (tmpString) : (""));
        closureDTO->gObjectName = g_strdup (tmpString);

        tmpString = signal_name;
        tmpString = ((tmpString) ? (tmpString) : (""));
        closureDTO->signal = g_strdup (tmpString);

        if (GTK_IS_BUILDABLE (object)) {
                closureDTO->widgetId = gtk_buildable_get_name (GTK_BUILDABLE (object));
        }
        else {
                closureDTO->widgetId = 0;
        }

        /*
         * https://developer.gnome.org/gobject/stable/gobject-Closures.html#g-cclosure-new
         */
        GClosure *closure = g_cclosure_new (G_CALLBACK (handler), closureDTO, gclosureUserDataDelete);

        g_closure_set_marshal (closure, gClosureMarshal);
        g_signal_connect_closure (object, signal_name, closure, FALSE);
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

        std::string signalNameCopy = dto->signal;
        std::string handlerCopy = dto->handler;
        std::string gObjectNameCopy = dto->gObjectName;
        std::string widgetIdCopy = (dto->widgetId) ? (dto->widgetId) : ("");

        ISignalAdapter *signalParametersAdapter = 0;
//         TODO signal dapters!
//        SignalAdapterVector adapters = dto->app->getCurrentUnit ()->getSignalAdapters();

//        for (ISignalAdapter *a : adapters) {
//                if (a->getSignal () == signalNameCopy) {
//                        signalParametersAdapter = a;

//                        if (a->getGObjectName () == gObjectNameCopy) {
//                                signalParametersAdapter = a;

//                                // Full match;
//                                if (a->getWidgetId () == widgetIdCopy) {
//                                        signalParametersAdapter = a;
//                                        break;
//                                }
//                        }
//                }
//        }

        if (signalParametersAdapter) {
                BOOST_LOG (lg) << "Signal adapter has been found : signal : [" << signalParametersAdapter->getSignal ()
                               << "], gObjectname : [" << signalParametersAdapter->getGObjectName ()
                               << "], widgetId : [" << signalParametersAdapter->getWidgetId () << "]";
        }
        else {
                BOOST_LOG (lg) << "No signal adapter has been found, passing raw signal parameters.";
        }

        typedef void (*HandlerType) (const std::string &,
                                     const Core::VariantVector &,
                                     AbstractAccessor *);

        GCClosure *cc = (GCClosure*) closure;
        HandlerType callback = (HandlerType) (marshal_data ? marshal_data : cc->callback);

        Core::VariantVector params;

        if (signalParametersAdapter) {
                params = signalParametersAdapter->adapt (n_param_values, param_values);
        }

        callback (dto->handler, params, dto->accessor);
}

/*--------------------------------------------------------------------------*/

void gclosureUserDataDelete (gpointer data, GClosure *closure)
{
        ClosureDTO *dto = static_cast <ClosureDTO *> (data);
        g_free (dto->gObjectName);
        g_free (dto->handler);
        g_free (dto->signal);
        free (dto);
}

} /* namespace GtkForms */
