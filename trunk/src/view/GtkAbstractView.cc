/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/regex.hpp>
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

        static void onIterateWidget (GtkWidget *widget, gpointer data);
        static void onPrintWidget (GtkWidget *widget, gpointer data);
        static bool nameMatches (std::string const &widgetName, std::string *inputName, std::string const &dataRange);
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

struct InputsSearchDTO {
        std::string dataRange;
        GtkAbstractView::InputMap inputs;
};

/*--------------------------------------------------------------------------*/

bool GtkAbstractView::Impl::nameMatches (std::string const &widgetName, std::string *inputName, std::string const &dataRange)
{
        boost::regex e;

        if (dataRange.empty ()) {
                e = boost::regex {"!(.*)"};
        }
        else {
                std::string copy = dataRange;
                boost::replace_all (copy, ".", "\\.*");
                boost::replace_all (copy, "*", ".*");
                e = boost::regex {"!(" + copy + ")"};
        }

        boost::smatch what;

        if (boost::regex_match (widgetName, what, e, boost::match_extra)) {
                if (what.size () == 2) {
                        *inputName = what[1];
                }

                return true;
        }

        return false;
}

/*--------------------------------------------------------------------------*/

//std::string GtkAbstractView::Impl::strip (std::string const &widgetName)
//{
//        return boost::erase_all_copy (widgetName, GtkAbstractView::INPUT_MARK_CHARACTER);
//}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::Impl::onIterateWidget (GtkWidget *widget, gpointer data)
{
        if (!widget || !GTK_IS_BUILDABLE (widget)) {
                return;
        }

        InputsSearchDTO *dto = static_cast <InputsSearchDTO *> (data);
        gchar const *buildableName = gtk_buildable_get_name (GTK_BUILDABLE (widget));

        if (buildableName) {
                std::string inputName;

                if (GtkAbstractView::Impl::nameMatches (buildableName, &inputName, dto->dataRange)) {
                        dto->inputs[inputName] = GTK_WIDGET (widget);
                }
        }

        if (GTK_IS_CONTAINER (widget)) {
                gtk_container_foreach (GTK_CONTAINER (widget), &GtkAbstractView::Impl::onIterateWidget, dto);
        }
}

/*--------------------------------------------------------------------------*/

GtkAbstractView::InputMap GtkAbstractView::getInputs (std::string const &dataRange)
{
        if (!GTK_IS_BUILDABLE (getUi ())) {
                BOOST_LOG (lg) << "Warn : UI is not of type GtkBuildable. Can not get ID then.";
                return GtkAbstractView::InputMap {};
        }

        InputsSearchDTO dto;
        dto.dataRange = dataRange;

        GtkBuildable *mainWidget = GTK_BUILDABLE (getUi ());
        gchar const *buildableName = gtk_buildable_get_name (mainWidget);

        if (buildableName) {
                std::string inputName;

                if (GtkAbstractView::Impl::nameMatches (buildableName, &inputName, dataRange)) {
                        dto.inputs[inputName] = GTK_WIDGET (mainWidget);
                }
        }

        if (!GTK_IS_CONTAINER (getUi ())) {
                return dto.inputs;
        }

        gtk_container_foreach (GTK_CONTAINER (mainWidget), &GtkAbstractView::Impl::onIterateWidget, &dto);

        return dto.inputs;
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::Impl::onPrintWidget (GtkWidget *widget, gpointer data)
{
        if (!widget || !GTK_IS_BUILDABLE (widget)) {
                return;
        }

        int *indent = static_cast <int *> (data);
        std::string id;

        for (int i = 0; i < *indent; ++i) {
                id += " ";
        }

        gchar const *buildableName = gtk_buildable_get_name (GTK_BUILDABLE (widget));
        gchar const *widgetName = gtk_widget_get_name (GTK_WIDGET (widget));
        BOOST_LOG (lg) << id << ((widgetName) ? (widgetName) : ("")) << ":" << ((buildableName) ? (buildableName) : (""));

        if (GTK_IS_CONTAINER (widget)) {
                int newIndent = *indent + 1;
                gtk_container_foreach (GTK_CONTAINER (widget), &GtkAbstractView::Impl::onPrintWidget, &newIndent);
        }
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::printStructure ()
{
        if (!GTK_IS_BUILDABLE (getUi ())) {
                BOOST_LOG (lg) << "UI is not of type GtkBuildable. Can not get ID then.";
        }

        GtkBuildable *mainWidget = GTK_BUILDABLE (getUi ());

        gchar const *buildableName = gtk_buildable_get_name (mainWidget);
        gchar const *widgetName = gtk_widget_get_name (GTK_WIDGET (mainWidget));
        BOOST_LOG (lg) << ((widgetName) ? (widgetName) : ("")) << ":" << ((buildableName) ? (buildableName) : (""));

        if (!GTK_IS_CONTAINER (getUi ())) {
                return;
        }

        int indent = 1;
        gtk_container_foreach (GTK_CONTAINER (mainWidget), &GtkAbstractView::Impl::onPrintWidget, &indent);
}

} // namespace GtkForms
