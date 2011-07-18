/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <gtk/gtk.h>
#include <ostream>
#include <Container.h>
#include <cassert>
#include <boost/lexical_cast.hpp>
#include <K202.h>
#include "ViewAdapter.h"
#include "tools/GValueVariant.h"

/*##########################################################################*/

namespace GtkForms {
using namespace Core;

/****************************************************************************/

struct Helper {

        Helper () : viewAdapter (NULL) {}

        std::string source;
        ViewAdapter *viewAdapter;
};

/**
 * Handler uruchamiany przez GTK, ktory z kolei uruchamia k202 i za jego pomocą
 * uruchamia jakieś tam skrypty, które podamy w GLADE.
 * @param sourceCode Kod źródłowy handlera z glade.
 * @param context To jest VariantMap i idzie jako domain do k202.
 * @param paramVector Argumenty sygnału GTK skonwertowane na Variant. Odwołujemy się
 * za pomocą %1, %2, etc.
 */
void handler (std::string const &sourceCode,
              ViewAdapter *viewAdapter,
              Core::VariantVector const &paramVector)
{
#if 0
        std::cerr << "handler here : " << sourceCode << std::endl;

        for (VariantVector::const_iterator i = paramVector.begin (); i != paramVector.end (); ++i) {
                std::cerr << "param : " << i->toString () << std::endl;
        }
#endif

        k202::K202 *k = viewAdapter->myK202Script ();
        Core::Variant domain = Core::Variant (viewAdapter->getContext ().get ());
        Variant ret = k->run (sourceCode, domain, paramVector);

#if 0
        std::cerr << ret << std::endl;
#endif

        Wrapper::BeanWrapper *bw = viewAdapter->myBeanWrapper ();
        Common::Context ctx;

        for (MappingVector::const_iterator i = viewAdapter->getMappings ()->begin (); i != viewAdapter->getMappings ()->end (); ++i) {
                bw->setWrappedObject (ret);
                Variant modelProperty = bw->get ((*i)->getModelProp (), &ctx); // Discard errors

                if (modelProperty.isNone ()) {
                        continue;
                }

                bw->setWrappedObject (domain);
                bw->set ((*i)->getViewProp (), modelProperty, &ctx);
        }
}

/**
 * Wywoływane przez GTK kiedy closure nie jest już potrzebne. Kasujemy Helper.
 * @param data
 * @param closure
 */
static void  onClosureDelete (gpointer data,  GClosure *closure)
{
        Helper *helper = static_cast <Helper *> (closure->data);
        delete helper;
}

/**
 * Marshaler to jest specjalna funkcja która jest częścią closure i wskaźnik do niej jest
 * podawany przy tworzeni closure. Ma za zadanie zmienić typ wejściowy i wyjściowy handlera
 * z GValues na coś innego.
 * @param closure
 * @param return_value
 * @param n_param_values
 * @param param_values
 * @param invocation_hint
 * @param marshal_data
 */
static void gClosureMarshal (GClosure *closure,
                             GValue *return_value,
                             guint n_param_values,
                             const GValue *param_values,
                             gpointer invocation_hint,
                             gpointer marshal_data)
{
#if 0
        std::cerr << "Marshaler here. No of params : " << n_param_values << std::endl;
#endif

        typedef void (*HandlerType) (std::string const &sourceCode,
                                     ViewAdapter *viewAdapter,
                                     Core::VariantVector const &paramVector);

        GCClosure *cc = (GCClosure*) closure;
        HandlerType callback;

        callback = (HandlerType) (marshal_data ? marshal_data : cc->callback);

        Core::VariantVector params;

        for (unsigned int i = 0; i < n_param_values; ++i) {
                params.push_back (GtkForms::gValueToVariant (param_values + i));
        }

        Helper *helper = static_cast <Helper *> (closure->data);
        callback (helper->source, helper->viewAdapter, params);
}

/**
 * To jest handler podawany do gtk_builder_connect_signals_full. Odpala się
 * per sygnał.
 */
static void myConnectFunc (GtkBuilder *builder,
                           GObject *object,
                           const gchar *signal_name,
                           const gchar *handler_name,
                           GObject *connect_object,
                           GConnectFlags flags,
                           gpointer user_data)
{
#if 0
        std::cerr << "Connecting : " << signal_name << ", " << handler_name << ", " << user_data << std::endl;
#endif

        Helper *helper = new Helper ();
//        helper->context = static_cast <Core::VariantMap *> (user_data);
        helper->viewAdapter = static_cast <ViewAdapter *> (user_data);
        helper->source = handler_name;

        // Podać jako parametr tam gdzie user_data
        GClosure *closure = g_cclosure_new (G_CALLBACK (handler), helper, onClosureDelete);

        g_closure_set_marshal (closure, gClosureMarshal);
        g_signal_connect_closure  (object, signal_name, closure, FALSE);
}

/****************************************************************************/

GtkWidget *ViewAdapter::create ()
{
#if 0
        std::cerr << "GtkBuilderFactory::create " << this << std::endl;
#endif

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

                gtk_builder_connect_signals_full (builder, myConnectFunc, this);
                window = GTK_WIDGET (gtk_builder_get_object (builder, widgetName.c_str ()));
                g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), &window);
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

/****************************************************************************/

Ptr <WidgetAdapter> ViewAdapter::getWidget (const std::string &name) const
{
        GtkBuilder *builder = gtkBuilderAdapter->getBuilder ();

        if (!builder) {
                return Ptr <WidgetAdapter> ();
        }

        GtkWidget *widget = GTK_WIDGET (gtk_builder_get_object (builder, name.c_str ()));

        if (!widget) {
                return Ptr <WidgetAdapter> ();
        }

        return boost::make_shared <WidgetAdapter> (widget);
}

/****************************************************************************/

Core::Variant ViewAdapter::get (const std::string &name) const
{
        Ptr <WidgetAdapter> a = getWidget (name);
        return ((a) ? (Core::Variant (a)) : (Core::Variant ()));
}

/****************************************************************************/

Wrapper::BeanWrapper *ViewAdapter::myBeanWrapper () const
{
        if (!beanWrapper) {
                beanWrapper = Wrapper::BeanWrapper::create ();
        }

        return beanWrapper.get ();
}

/****************************************************************************/

k202::K202 *ViewAdapter::myK202Script () const
{
        if (k202) {
                return k202.get ();
        }

        return k202::K202::instance ();
}

}
