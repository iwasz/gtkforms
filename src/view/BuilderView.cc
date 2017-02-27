/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "BuilderView.h"
#include "App.h"
#include "Logging.h"
#include "signalAdapter/ISignalAdapter.h"
#include <boost/algorithm/string/split.hpp>
#include <gtk/gtk.h>

namespace GtkForms {
static src::logger_mt &lg = logger::get ();

extern "C" {
/*
 * Helper class for passing required data between user code and GObject closures.
 */
struct ClosureDTO {
        char *signal = nullptr;
        char *gObjectName = nullptr;
        char const *widgetId = nullptr;
        char *handler = nullptr;
        AbstractAccessor *accessor = nullptr;
        SignalAdapterVector *signalAdapters = nullptr;
};

struct ConnectDTO {
        AbstractAccessor *accessor = nullptr;
        SignalAdapterVector *signalAdapters = nullptr;
};

/*
 * https://developer.gnome.org/gtk3/stable/GtkBuilder.html#GtkBuilderConnectFunc
 */
void myConnectFunc (GtkBuilder *builder, GObject *object, const gchar *signal_name, const gchar *handler_name, GObject *connect_object, GConnectFlags flags,
                    gpointer user_data);

void gClosureMarshal (GClosure *closure, GValue *return_value, guint n_param_values, const GValue *param_values, gpointer invocation_hint,
                      gpointer marshal_data);

void gclosureUserDataDelete (gpointer data, GClosure *closure);

void handler (const std::string &sourceCode, const Core::VariantVector &paramVector, AbstractAccessor *accessor);

} // extern "C"

/*****************************************************************************/

struct BuilderView::Impl {
        static void onUserClickedQuit (GtkWidget *object, gpointer userData);

        GtkWidget *widget = nullptr;
        bool deleteUiFile = false;
        GtkBuilder *builder = nullptr;
        bool loaded = false;
        ConnectDTO connectDTO;
        Context *context = nullptr;
};

/*****************************************************************************/

BuilderView::BuilderView () : loadWholeFile (false) { impl = new Impl; }

/*****************************************************************************/

BuilderView::~BuilderView () { delete impl; }

/*****************************************************************************/

/*
 * Z tego co pamiętam, to gtk_builder tworzy singletony.
 */
void BuilderView::loadUi (App *app)
{
        BOOST_LOG (lg) << "BuilderView::loadUi : \033[35m[" << name << "]\033[0m";
        clearInternalState ();
        setConfig (app->getConfig ());
        impl->context = &app->getContext ();

        if (impl->loaded) {
                return;
        }

        if (file.empty ()) {
                throw Core::Exception ("You didn't specify a file name to load in this UiFile factory object.");
        }

        GError *err = NULL;

        impl->builder = gtk_builder_new ();

        if (loadWholeFile) {
                gtk_builder_add_from_file (impl->builder, file.c_str (), &err);
        }
        else {
                Core::StringVector alsoLoadList;
                if (!alsoLoad.empty ()) {
                        boost::split (alsoLoadList, alsoLoad, boost::is_any_of (", "), boost::token_compress_on);
                }

                gchar *objectIds[alsoLoadList.size () + 2];

                for (int i = 0; i < alsoLoadList.size (); ++i) {
                        objectIds[i] = const_cast<char *> (alsoLoadList[i].c_str ());
                }

                objectIds[alsoLoadList.size ()] = const_cast<char *> (name.c_str ());
                objectIds[alsoLoadList.size () + 1] = 0;

                gtk_builder_add_objects_from_file (impl->builder, file.c_str (), objectIds, &err);
        }

        if (err) {
                throw Core::Exception ("gtk_builder_add_from_file returned an error. Error message is : [" + std::string (err->message) + "]");
        }

        impl->loaded = true;
        impl->widget = GTK_WIDGET (gtk_builder_get_object (impl->builder, name.c_str ()));

        if (!impl->widget) {
                throw Core::Exception ("No widget with name : [" + name + "] was found in file : [" + file + "].");
        }

        populateInputMap ();

        if (GTK_IS_WINDOW (getUi ())) {
                g_signal_connect (GTK_WINDOW (getUi ()), "destroy", G_CALLBACK (&BuilderView::Impl::onUserClickedQuit), this);
        }
}

/*--------------------------------------------------------------------------*/

void BuilderView::Impl::onUserClickedQuit (GtkWidget *object, gpointer userData)
{
        BuilderView *view = static_cast<BuilderView *> (userData);
        AbstractController *controller = view->getControllerFromUi ();
        App *app = controller->getApp ();

        // Close this controller
        if (controller->getParent ()) {
                app->close (controller);
        }
        else {
                app->userQuitRequest ();
        }
}

/*--------------------------------------------------------------------------*/

// TODO sporo (jeśli nie cały) poniższego kodu powinba być przeniesiona do AbstractView.
void BuilderView::destroyUi ()
{
        BOOST_LOG (lg) << "BuilderView::destroyUi : \033[35m[" << name << "]\033[0m";

        runDecorators (IPageDecorator::PRE_CLOSE, impl->context);
        clearInternalState ();

        if (!impl->widget) {
                return;
        }

        hide ();

        GtkWidget *container = gtk_widget_get_parent (impl->widget);
        // That's strange, but gtk_widget_get_ancestor will return the widget itself if it is a GtkContiner.

        if (container) {
                container = gtk_widget_get_ancestor (container, GTK_TYPE_CONTAINER);
        }

        if (container) {
                BOOST_LOG (lg) << "Attempting to remove a widget : "
                               << "gtk_widget_get_name : [" << gtk_widget_get_name (impl->widget) << "], gtk_buildable_get_name : ["
                               << gtk_buildable_get_name (GTK_BUILDABLE (impl->widget)) << "], "
                               << "from container : gtk_widget_get_name : [" << gtk_widget_get_name (container) << "], gtk_buildable_get_name of its child : ["
                               << gtk_buildable_get_name (GTK_BUILDABLE (container)) << "].";

                gtk_container_remove (GTK_CONTAINER (container), impl->widget);
        }

#if 0
        BOOST_LOG (lg) << " -GtkBuilderView::destroyUi : [" << name << "]";
#endif

        //        Sholud I? Or shouldn't?
        //        if (GTK_IS_WINDOW (impl->widget)) {
        //                g_object_unref (impl->widget);
        //        }

        impl->widget = nullptr;

        g_object_unref (impl->builder);
        impl->loaded = false;
        impl->builder = nullptr;
}

/*--------------------------------------------------------------------------*/

bool BuilderView::isLoaded () const { return static_cast<bool> (impl->widget); }

/*--------------------------------------------------------------------------*/

GObject *BuilderView::getUi () { return G_OBJECT (impl->widget); }

/*--------------------------------------------------------------------------*/

GObject *BuilderView::getUi (std::string const &name) { return gtk_builder_get_object (impl->builder, name.c_str ()); }

/*--------------------------------------------------------------------------*/

GObject *BuilderView::getUiOrThrow (std::string const &name)
{
        GObject *obj = getUi (name);

        if (!obj) {
                throw Core::Exception ("GtkAbstractView::getGObject could not find an object in UI. Ui file : [" + file + "], object name : ["
                                       + std::string (name) + "].");
        }

        return obj;
}

/*****************************************************************************/

void BuilderView::connectSignals (AbstractAccessor *accessor)
{
        impl->connectDTO.accessor = accessor;
        impl->connectDTO.signalAdapters = &getSignalAdapters ();
        // https://developer.gnome.org/gtk3/stable/GtkBuilder.html#gtk-builder-connect-signals-full
        gtk_builder_connect_signals_full (impl->builder, myConnectFunc, static_cast<gpointer> (&impl->connectDTO));

        BOOST_LOG (lg) << "builder = [" << impl->builder << "], myConnectFunc = [" << myConnectFunc << "], connectDTO = [" << &impl->connectDTO << "]";
}

/*--------------------------------------------------------------------------*/

void myConnectFunc (GtkBuilder *builder, GObject *object, const gchar *signal_name, const gchar *handler_name, GObject *connect_object, GConnectFlags flags,
                    gpointer user_data)
{
        BOOST_LOG (lg) << "Connecting : " << signal_name << ", " << handler_name /*<< ", DTO : [" << (void *)&UiFile::Impl::closureDTO << "]"*/;

        // Skopiować napis z signal_name
        ClosureDTO *closureDTO = static_cast<ClosureDTO *> (malloc (sizeof (ClosureDTO)));

        char const *tmpString = handler_name;
        tmpString = ((tmpString) ? (tmpString) : (""));
        closureDTO->handler = g_strdup (tmpString);

        ConnectDTO *connectDTO = static_cast<ConnectDTO *> (user_data);
        closureDTO->accessor = connectDTO->accessor;
        closureDTO->signalAdapters = connectDTO->signalAdapters;

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

void gClosureMarshal (GClosure *closure, GValue *return_value, guint n_param_values, const GValue *param_values, gpointer invocation_hint,
                      gpointer marshal_data)
{
        BOOST_LOG (lg) << "Marshaler here. closure : [" << (void *)closure << "], n_param_values : [" << n_param_values << "], invocation_hint : ["
                       << (void *)invocation_hint << "], marshal_data [" << (void *)marshal_data << "]";

        ClosureDTO *dto = static_cast<ClosureDTO *> (closure->data);

        std::string signalNameCopy = dto->signal;
        std::string handlerCopy = dto->handler;
        std::string gObjectNameCopy = dto->gObjectName;
        std::string widgetIdCopy = (dto->widgetId) ? (dto->widgetId) : ("");

        ISignalAdapter *signalParametersAdapter = nullptr;
        SignalAdapterVector *adapters = dto->signalAdapters;

        if (adapters) {
                for (ISignalAdapter *a : *adapters) {
                        signalParametersAdapter = (a->getSignal () == signalNameCopy) ? (a) : (nullptr);

                        if (!a->getGObjectName ().empty ()) {
                                signalParametersAdapter = (a->getGObjectName () == gObjectNameCopy) ? (a) : (nullptr);
                        }

                        if (!a->getWidgetId ().empty ()) {
                                signalParametersAdapter = (a->getWidgetId () == widgetIdCopy) ? (a) : (nullptr);
                        }

                        if (signalParametersAdapter) {
                                break;
                        }
                }
        }

        if (signalParametersAdapter) {
                BOOST_LOG (lg) << "Signal adapter has been found : signal : [" << signalParametersAdapter->getSignal () << "], gObjectname : ["
                               << signalParametersAdapter->getGObjectName () << "], widgetId : [" << signalParametersAdapter->getWidgetId () << "]";
        }
        else {
                BOOST_LOG (lg) << "No signal adapter has been found, passing raw signal parameters.";
        }

        typedef void (*HandlerType) (const std::string &, const Core::VariantVector &, AbstractAccessor *);

        GCClosure *cc = (GCClosure *)closure;
        HandlerType callback = (HandlerType) (marshal_data ? marshal_data : cc->callback);

        Core::VariantVector params;

        if (signalParametersAdapter) {
                params = signalParametersAdapter->adapt (n_param_values, param_values);
        }

        callback (dto->handler, params, dto->accessor);
}

/*--------------------------------------------------------------------------*/

void handler (const std::string &sourceCode, const Core::VariantVector &paramVector, AbstractAccessor *accessor)
{
        Core::Variant domain{ accessor };
        k202::K202 *k202 = App::getK202 ();

        Core::Variant result = k202->run (sourceCode, domain, paramVector);
        BOOST_LOG (lg) << "handler here : [" << sourceCode << "]. The result : [" << result.toString () << "]";
}

/*--------------------------------------------------------------------------*/

void gclosureUserDataDelete (gpointer data, GClosure *closure)
{
        ClosureDTO *dto = static_cast<ClosureDTO *> (data);
        g_free (dto->gObjectName);
        g_free (dto->handler);
        g_free (dto->signal);
        free (dto);
}
}
