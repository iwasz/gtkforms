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
        bool deleteUiFile = false;

        static void onIterateWidget (GtkWidget *widget, gpointer data);
        static void onPrintWidget (GtkWidget *widget, gpointer data);
        static bool nameMatches (std::string const &widgetName, std::string *inputName, std::string const &dataRange, bool outputs);
};

/*--------------------------------------------------------------------------*/

GtkAbstractView::GtkAbstractView () : uiFile {nullptr}
{
        impl = new Impl;
}

/*--------------------------------------------------------------------------*/

GtkAbstractView::~GtkAbstractView ()
{
        if (impl->deleteUiFile) {
                delete uiFile;
        }
        delete impl;
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::loadUi (App *app)
{
        if (!ui.empty () && !uiFile) {
                uiFile = new UiFile;
                uiFile->setFile (ui);
                impl->deleteUiFile = true;
        }

        if (!uiFile) {
                throw Core::Exception ("No UiFile object set inside GtkAbstractView.");
        }

        if (impl->widget) {
                return;
        }

#if 0
        BOOST_LOG (lg) << " +GtkAbstractView::loadUi : [" << name << "]";
#endif

        uiFile->load (app);
        impl->widget = GTK_WIDGET (gtk_builder_get_object (uiFile->getBuilder (), name.c_str ()));

        if (!impl->widget) {
                throw Core::Exception ("No widget with name : [" + name + "] was found in file : [" + uiFile->getFile () + "].");
        }

        gtk_widget_show_all (impl->widget);
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
        if (!impl->widget) {
                return;
        }

#if 0
        BOOST_LOG (lg) << " -GtkAbstractView::destroyUi : [" << name << "]";
#endif

        hide ();
        gtk_widget_destroy (impl->widget);
        impl->widget = 0;
        uiFile->destroy ();
}

/*--------------------------------------------------------------------------*/

GObject *GtkAbstractView::getUi ()
{
        return G_OBJECT (impl->widget);
}

/*--------------------------------------------------------------------------*/

bool GtkAbstractView::isLoaded () const
{
        return static_cast <bool> (impl->widget);
}

/*--------------------------------------------------------------------------*/

GObject *GtkAbstractView::getUiOrThrow (std::string const &name)
{
        GObject *obj =  gtk_builder_get_object (uiFile->getBuilder (), name.c_str ());

        if (!obj) {
                throw Core::Exception ("GtkAbstractView::getGObject could not find ARBITRARY object in UI. Ui file : [" + uiFile->getFile () + "], object name : [" + std::string (name) + "].");
        }

        return obj;
}

/*--------------------------------------------------------------------------*/

GObject *GtkAbstractView::getUi (std::string const &name)
{
        return gtk_builder_get_object (uiFile->getBuilder (), name.c_str ());
}

/*--------------------------------------------------------------------------*/

struct InputsSearchDTO {
        std::string dataRange;
        GtkAbstractView::InputMap inputs;
        bool outputs = false;
};

/*--------------------------------------------------------------------------*/

bool GtkAbstractView::Impl::nameMatches (std::string const &widgetName, std::string *inputName, std::string const &dataRange, bool outputs)
{
#if 0
        BOOST_LOG (lg) << widgetName;
#endif

        boost::regex e;

        if (dataRange.empty ()) {
                if (outputs) {
                        e = boost::regex {"[!>](.*)"};
                }
                else {
                        e = boost::regex {"[!<](.*)"};
                }
        }
        else {
                std::string copy = dataRange;
                boost::replace_all (copy, ".", "\\.*");
                boost::replace_all (copy, "*", ".*");

                if (outputs) {
                        e = boost::regex {"[!>](" + copy + ")"};
                }
                else {
                        e = boost::regex {"[!<](" + copy + ")"};
                }
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

                if (GtkAbstractView::Impl::nameMatches (buildableName, &inputName, dto->dataRange, dto->outputs)) {
                        dto->inputs[inputName] = GTK_WIDGET (widget);
                }
        }

        if (GTK_IS_CONTAINER (widget)) {
                gtk_container_foreach (GTK_CONTAINER (widget), &GtkAbstractView::Impl::onIterateWidget, dto);
        }
}

/*--------------------------------------------------------------------------*/

GtkAbstractView::InputMap GtkAbstractView::getInputs (std::string const &dataRange, bool outputs)
{
        if (!GTK_IS_BUILDABLE (getUi ())) {
                BOOST_LOG (lg) << "Warn : UI is not of type GtkBuildable. Can not get ID then.";
                return GtkAbstractView::InputMap {};
        }

        InputsSearchDTO dto;
        dto.dataRange = dataRange;
        dto.outputs = outputs;

        GtkBuildable *mainWidget = GTK_BUILDABLE (getUi ());
        gchar const *buildableName = gtk_buildable_get_name (mainWidget);

        if (buildableName) {
                std::string inputName;

                if (GtkAbstractView::Impl::nameMatches (buildableName, &inputName, dataRange, outputs)) {
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
