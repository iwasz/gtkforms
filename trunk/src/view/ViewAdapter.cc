/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <gtk/gtk.h>
#include <ostream>
#include <container/Container.h>
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
              Core::VariantVector const &paramVector,
              Core::VariantMap const &argsMap)
{
#if 0
        std::cerr << "handler here : " << sourceCode << std::endl;

        for (VariantVector::const_iterator i = paramVector.begin (); i != paramVector.end (); ++i) {
                std::cerr << "param : " << i->toString () << std::endl;
        }
#endif

        k202::K202 *k = viewAdapter->myK202Script ();
        Core::Variant domain = Core::Variant (viewAdapter->getContext ().get ());
        Variant ret = k->run (sourceCode, domain, paramVector, argsMap);

#if 0
        std::cerr << "Model : " << ret << std::endl;
#endif

        viewAdapter->getMapper ()->m2v (ret);
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
                                     Core::VariantVector const &paramVector,
                                     Core::VariantMap const &argsMap);

        GCClosure *cc = (GCClosure*) closure;
        HandlerType callback;

        callback = (HandlerType) (marshal_data ? marshal_data : cc->callback);

        Core::VariantVector params;

        for (unsigned int i = 1; i < n_param_values; ++i) {
                params.push_back (GtkForms::gValueToVariant (param_values + i));
        }

        Core::VariantMap args;

        if (n_param_values > 0 && G_VALUE_HOLDS_OBJECT (param_values)) {
                GObject *object = G_OBJECT (g_value_get_object (param_values));
                Ptr <GObjectAdapter> adapter = boost::make_shared <GObjectAdapter> (object);
                args["this"] = Variant (adapter);
        }

        Helper *helper = static_cast <Helper *> (closure->data);
        callback (helper->source, helper->viewAdapter, params, args);
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

        window = NULL;
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

        return window;
}
/****************************************************************************/

void ViewAdapter::show ()
{
        if (!gtk_widget_get_visible (window)) {
                gtk_widget_show_all (window);
        }
}

/****************************************************************************/

Ptr <GObjectAdapter> ViewAdapter::getGObject (const std::string &name) const
{
        GtkBuilder *builder = gtkBuilderAdapter->getBuilder ();

        if (!builder) {
                return Ptr <GObjectAdapter> ();
        }

//        GtkWidget *widget = GTK_WIDGET (gtk_builder_get_object (builder, name.c_str ()));
        GObject *object = gtk_builder_get_object (builder, name.c_str ());

        if (!object) {
                return Ptr <GObjectAdapter> ();
        }

        return boost::make_shared <GObjectAdapter> (object);
}

/****************************************************************************/

Core::Variant ViewAdapter::get (const std::string &name) const
{
        Ptr <GObjectAdapter> a = getGObject (name);
        return ((a) ? (Core::Variant (a)) : (Core::Variant ()));
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
