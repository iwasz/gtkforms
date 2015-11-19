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
#include "RegexHelper.h"

namespace GtkForms {
static src::logger_mt &lg = logger::get ();

struct GtkAbstractView::Impl {
        enum WidgetType { NOT_RELEVANT, INPUT, OUTPUT, BOTH };

        struct InputsSearchDTO {
                GtkAbstractView::InputMap *inputs = nullptr;
                GtkAbstractView::InputMap *outputs = nullptr;
                Config const *config = nullptr;
        };

        static void onIterateWidget (GtkWidget *widget, gpointer data);
        static void onPrintWidget (GtkWidget *widget, gpointer data);
        static WidgetType getWidgetType (std::string const &widgetNameWithPrefix, std::string *widgetName);
        static bool storeWidget (InputsSearchDTO *dto, std::string const &widghetName, GtkWidget *widget);
        static void storeWidget (InputsSearchDTO *dto, GtkWidget *widget);

        GtkWidget *widget = 0;
        bool deleteUiFile = false;
        GtkAbstractView::InputMap inputWidgetsMap;
        GtkAbstractView::InputMap outputWidgetsMap;
        Config const *config = 0;
};

/*--------------------------------------------------------------------------*/

GtkAbstractView::GtkAbstractView () : uiFile{ nullptr } { impl = new Impl; }

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

        impl->config = app->getConfig ();
        uiFile->load (app);
        impl->widget = GTK_WIDGET (gtk_builder_get_object (uiFile->getBuilder (), name.c_str ()));

        if (!impl->widget) {
                throw Core::Exception ("No widget with name : [" + name + "] was found in file : [" + uiFile->getFile () + "].");
        }

        populateInputMap ();
        gtk_widget_show (impl->widget);
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::show () { gtk_widget_show (impl->widget); }

/*--------------------------------------------------------------------------*/

void GtkAbstractView::hide () { gtk_widget_hide (impl->widget); }

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

GObject *GtkAbstractView::getUi () { return G_OBJECT (impl->widget); }

/*--------------------------------------------------------------------------*/

bool GtkAbstractView::isLoaded () const { return static_cast<bool> (impl->widget); }

/*--------------------------------------------------------------------------*/

GObject *GtkAbstractView::getUiOrThrow (std::string const &name)
{
        GObject *obj = gtk_builder_get_object (uiFile->getBuilder (), name.c_str ());

        if (!obj) {
                throw Core::Exception ("GtkAbstractView::getGObject could not find ARBITRARY object in UI. Ui file : [" + uiFile->getFile ()
                                       + "], object name : [" + std::string (name) + "].");
        }

        return obj;
}

/*--------------------------------------------------------------------------*/

GObject *GtkAbstractView::getUi (std::string const &name) { return gtk_builder_get_object (uiFile->getBuilder (), name.c_str ()); }

/*****************************************************************************/

bool GtkAbstractView::Impl::storeWidget (InputsSearchDTO *dto, std::string const &widgetName, GtkWidget *widget)
{
        std::string inputName;
        bool stored = false;

        Impl::WidgetType t = Impl::getWidgetType (widgetName, &inputName);
        if (t == Impl::WidgetType::INPUT || t == Impl::WidgetType::BOTH) {
                dto->inputs->insert (std::make_pair (inputName, widget));
                stored = true;
        }
        else if (t == Impl::WidgetType::OUTPUT || t == Impl::WidgetType::BOTH) {
                dto->outputs->insert (std::make_pair (inputName, widget));
                stored = true;
        }

        return stored;
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::Impl::storeWidget (InputsSearchDTO *dto, GtkWidget *widget)
{
        if (!GTK_IS_BUILDABLE (widget)) {
                return;
        }

        gchar const *widgetName = gtk_widget_get_name (widget);
        bool stored = false;

        if (widgetName) {
#if 0
                BOOST_LOG (lg) << widgetName;
#endif

                stored = storeWidget (dto, widgetName, GTK_WIDGET (widget));
        }

        if (!stored && dto->config->useWidgetId) {
                gchar const *buildableName = gtk_buildable_get_name (GTK_BUILDABLE (widget));

                if (buildableName) {
#if 0
                        BOOST_LOG (lg) << buildableName;
#endif
                        stored = storeWidget (dto, buildableName, widget);
                }
        }
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::populateInputMap ()
{
        if (!GTK_IS_BUILDABLE (getUi ()) || !GTK_IS_WIDGET (getUi ())) {
                BOOST_LOG (lg) << "Warn : UI is not of type GtkBuildable. Can not get ID then.";
                return;
        }

        Impl::InputsSearchDTO dto;
        dto.inputs = &impl->inputWidgetsMap;
        dto.outputs = &impl->outputWidgetsMap;
        dto.config = impl->config;

        Impl::storeWidget (&dto, GTK_WIDGET (getUi ()));

        if (!GTK_IS_CONTAINER (getUi ())) {
                return;
        }

        gtk_container_foreach (GTK_CONTAINER (getUi ()), &GtkAbstractView::Impl::onIterateWidget, &dto);
}

/*--------------------------------------------------------------------------*/

void GtkAbstractView::Impl::onIterateWidget (GtkWidget *widget, gpointer data)
{
        if (!widget || !GTK_IS_BUILDABLE (widget)) {
                return;
        }

        InputsSearchDTO *dto = static_cast<InputsSearchDTO *> (data);
        storeWidget (dto, widget);

        if (GTK_IS_CONTAINER (widget)) {
                gtk_container_foreach (GTK_CONTAINER (widget), &GtkAbstractView::Impl::onIterateWidget, dto);
        }
}

/*--------------------------------------------------------------------------*/

GtkAbstractView::Impl::WidgetType GtkAbstractView::Impl::getWidgetType (std::string const &widgetNameWithPrefix, std::string *widgetName)
{
        if (widgetNameWithPrefix.empty ()) {
                return WidgetType::NOT_RELEVANT;
        }

        char prefix = widgetNameWithPrefix[0];

        if (prefix == '!') {
                *widgetName = std::string (widgetNameWithPrefix.c_str () + 1);
                return WidgetType::BOTH;
        }

        if (prefix == '>') {
                *widgetName = std::string (widgetNameWithPrefix.c_str () + 1);
                return WidgetType::OUTPUT;
        }

        if (prefix == '<') {
                *widgetName = std::string (widgetNameWithPrefix.c_str () + 1);
                return WidgetType::INPUT;
        }

        *widgetName = widgetNameWithPrefix;
        return WidgetType::NOT_RELEVANT;
}

/*****************************************************************************/

GtkAbstractView::InputMap GtkAbstractView::getInputs (std::string const &dataRange, bool outputs)
{
        if (!GTK_IS_BUILDABLE (getUi ())) {
                BOOST_LOG (lg) << "Warn : UI is not of type GtkBuildable. Can not get ID then.";
                return GtkAbstractView::InputMap{};
        }

        // Zwróć cały zakres.
        if (dataRange.empty ()) {
                if (outputs) {
                        return impl->outputWidgetsMap;
                }
                else {
                        return impl->inputWidgetsMap;
                }
        }

        GtkAbstractView::InputMap resultMap;
        GtkAbstractView::InputMap const *mapToSearch;

        mapToSearch = (outputs) ? (&impl->outputWidgetsMap) : (&impl->inputWidgetsMap);

        for (GtkAbstractView::InputMap::value_type const &i : *mapToSearch) {
                std::string const &name = i.first;
                GtkWidget *widget = i.second;

                if (RegexHelper::inputNameMatches (name, dataRange)) {
                        resultMap.insert (std::make_pair (name, widget));
                }

        }

        return resultMap;
}

/*--------------------------------------------------------------------------*/
#if 0
void GtkAbstractView::Impl::onIterateWidget (GtkWidget *widget, gpointer data)
{
        if (!widget || !GTK_IS_BUILDABLE (widget)) {
                return;
        }

        InputsSearchDTO *dto = static_cast <InputsSearchDTO *> (data);
        gchar const *buildableName = gtk_buildable_get_name (GTK_BUILDABLE (widget));

        if (buildableName) {
                std::string inputName;

                if (RegexHelper::inputNameMatches (buildableName, &inputName, dto->dataRange, dto->outputs)) {
                        dto->inputs[inputName] = GTK_WIDGET (widget);
                }
        }

        if (GTK_IS_CONTAINER (widget)) {
                gtk_container_foreach (GTK_CONTAINER (widget), &GtkAbstractView::Impl::onIterateWidget, dto);
        }
}
#endif

/*****************************************************************************/

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

/*--------------------------------------------------------------------------*/

void GtkAbstractView::Impl::onPrintWidget (GtkWidget *widget, gpointer data)
{
        if (!widget || !GTK_IS_BUILDABLE (widget)) {
                return;
        }

        int *indent = static_cast<int *> (data);
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

} // namespace GtkForms
