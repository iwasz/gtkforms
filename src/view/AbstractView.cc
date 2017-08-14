/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "AbstractView.h"
#include "App.h"
#include "Context.h"
#include "Logging.h"
#include "RegexHelper.h"
#include "mapping/GValueVariant.h"
#include <Tiliae.h>
#include <cstdint>
#include <gtk/gtk.h>

namespace GtkForms {
static src::logger_mt &lg = logger::get ();

struct AbstractView::Impl {

        ~Impl () { /*delete mappingsByInputCache;*/}

        enum WidgetType { NOT_RELEVANT = 0x0, INPUT = 0x01, OUTPUT = 0x02, SLOT = 0x04 };

        struct InputsSearchDTO {
                AbstractView::WidgetMap *inputs = nullptr;
                AbstractView::WidgetMap *outputs = nullptr;
                AbstractView::WidgetMap *slots = nullptr;
                Config const *config = nullptr;
        };

        static void onIterateWidget (GtkWidget *widget, gpointer data);
        static void onPrintWidget (GtkWidget *widget, gpointer data);
        static uint32_t getWidgetType (std::string const &widgetNameWithPrefix, std::string *widgetName);
        static bool storeWidget (InputsSearchDTO *dto, std::string const &widghetName, GtkWidget *widget);
        static void storeWidget (InputsSearchDTO *dto, GtkWidget *widget);

        AbstractView::WidgetMap inputWidgetsMap;
        AbstractView::WidgetMap outputWidgetsMap;
        AbstractView::WidgetMap slotWidgetsMap;
        Config const *config = nullptr;
        AbstractController *controller = nullptr;
        bool deleteOnClose = false;
};

/*--------------------------------------------------------------------------*/

AbstractView::AbstractView () { impl = new Impl; }

/*--------------------------------------------------------------------------*/

AbstractView::~AbstractView () { delete impl; }

/*--------------------------------------------------------------------------*/

void AbstractView::show () { gtk_widget_show_all (GTK_WIDGET (getUi ())); }

/*--------------------------------------------------------------------------*/

void AbstractView::hide () { gtk_widget_hide (GTK_WIDGET (getUi ())); }

/*--------------------------------------------------------------------------*/

AbstractView *AbstractView::loadView (ViewsToOpen::ViewSlot const &vs, AbstractController *controller, Container::BeanFactoryContainer *container)
{
        AbstractView *view = ocast<AbstractView *> (container->getBean (vs.view));
        // Ustawi w polu impl->controller
        view->setController (controller);
        view->loadUi (controller->getApp ());
        // Teraz ma załadowany widok, i ustawi
        view->setControllerToUi (controller);
        view->connectSignals (controller->getModelAccessor ());

        if (!vs.slot.empty ()) {
                if (!view->reparent (vs)) {
                        // TODO zmienić na warning
                        BOOST_LOG (lg) << "Warn : could not reparent!";
                }
        }
#if 0
        else {
                AbstractController *parent = controller->getParent ();

                if (!parent) {
                        goto done;
                }

                ViewVector const &parentControllerViews = parent->getViews ();

                if (parentControllerViews.empty ()) {
                        goto done;
                }

                for (AbstractView *parentControllerView : parentControllerViews) {
                        if (parentControllerView->getName () == "mainView") {
                                GtkWindow *parentWindow = GTK_WINDOW (parentControllerView->getUi ());
                                GtkWindow *thisWindow = GTK_WINDOW (view->getUi ());
                                gtk_window_set_transient_for (thisWindow, parentWindow);
                                std::cerr << "=======================TRANSIENT==========================" << std::endl;
                                break;
                        }
                }
        }
done:
#endif

        view->runDecorators (IPageDecorator::PRE_SHOW, &controller->getApp ()->getContext ());
        view->show ();
        return view;
}

/*---------------------------------------------------------------------------*/

GObject *AbstractView::getUiOrThrow (std::string const &name)
{
        GObject *obj = getUi (name);

        if (!obj) {
                throw Core::Exception ("GtkAbstractView::getGObject could not find an object in UI. Oject name : [" + std::string (name) + "].");
        }

        return obj;
}

/*****************************************************************************/

void AbstractView::populateInputMap ()
{
        if (!GTK_IS_BUILDABLE (getUi ()) || !GTK_IS_WIDGET (getUi ())) {
                BOOST_LOG (lg) << "Warn : UI is not of type GtkBuildable. Can not get ID then.";
                return;
        }

        Impl::InputsSearchDTO dto;
        dto.inputs = &impl->inputWidgetsMap;
        dto.outputs = &impl->outputWidgetsMap;
        dto.slots = &impl->slotWidgetsMap;
        dto.config = impl->config;

        Impl::storeWidget (&dto, GTK_WIDGET (getUi ()));

        if (!GTK_IS_CONTAINER (getUi ())) {
                return;
        }

        gtk_container_foreach (GTK_CONTAINER (getUi ()), &AbstractView::Impl::onIterateWidget, &dto);
}

/*--------------------------------------------------------------------------*/

void AbstractView::Impl::onIterateWidget (GtkWidget *widget, gpointer data)
{
        if (!widget || !GTK_IS_BUILDABLE (widget)) {
                return;
        }

        InputsSearchDTO *dto = static_cast<InputsSearchDTO *> (data);
        storeWidget (dto, widget);

        if (GTK_IS_CONTAINER (widget)) {
                gtk_container_foreach (GTK_CONTAINER (widget), &AbstractView::Impl::onIterateWidget, dto);
        }
}

/*--------------------------------------------------------------------------*/

bool AbstractView::Impl::storeWidget (InputsSearchDTO *dto, std::string const &widgetName, GtkWidget *widget)
{
        std::string inputName;
        bool stored = false;

        uint32_t t = Impl::getWidgetType (widgetName, &inputName);
        if (t & Impl::WidgetType::INPUT) {
                dto->inputs->insert (std::make_pair (inputName, widget));
                stored = true;
        }

        if (t & Impl::WidgetType::OUTPUT) {
                dto->outputs->insert (std::make_pair (inputName, widget));
                stored = true;
        }

        if (t & Impl::WidgetType::SLOT) {
                dto->slots->insert (std::make_pair (inputName, widget));
                stored = true;
        }

        return stored;
}

/*--------------------------------------------------------------------------*/

void AbstractView::Impl::storeWidget (InputsSearchDTO *dto, GtkWidget *widget)
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

uint32_t AbstractView::Impl::getWidgetType (std::string const &widgetNameWithPrefix, std::string *widgetName)
{
        if (widgetNameWithPrefix.empty ()) {
                return WidgetType::NOT_RELEVANT;
        }

        char prefix = widgetNameWithPrefix[0];
        *widgetName = std::string (widgetNameWithPrefix.c_str () + 1);

        if (prefix == '!') {
                return WidgetType::INPUT | WidgetType::OUTPUT;
        }

        if (prefix == '>') {
                return WidgetType::OUTPUT;
        }

        if (prefix == '<') {
                return WidgetType::INPUT;
        }

        if (prefix == '^') {
                return WidgetType::SLOT;
        }

        *widgetName = widgetNameWithPrefix;
        return WidgetType::NOT_RELEVANT;
}

/*****************************************************************************/

AbstractView::WidgetMap AbstractView::getInputOutputWidgets (std::string const &widgetNameRange, bool outputs)
{
        if (!GTK_IS_BUILDABLE (getUi ())) {
                BOOST_LOG (lg) << "Warn : UI is not of type GtkBuildable. Can not get ID then.";
                return AbstractView::WidgetMap{};
        }

        // Zwróć cały zakres.
        if (widgetNameRange.empty ()) {
                if (outputs) {
                        return impl->outputWidgetsMap;
                }
                else {
                        return impl->inputWidgetsMap;
                }
        }

        AbstractView::WidgetMap resultMap;
        AbstractView::WidgetMap const *mapToSearch;

        mapToSearch = (outputs) ? (&impl->outputWidgetsMap) : (&impl->inputWidgetsMap);

        for (AbstractView::WidgetMap::value_type const &i : *mapToSearch) {
                std::string const &name = i.first;
                GtkWidget *widget = i.second;

                if (RegexHelper::inputNameMatches (name, widgetNameRange)) {
                        resultMap.insert (std::make_pair (name, widget));
                }
        }

        return resultMap;
}

/*--------------------------------------------------------------------------*/

AbstractView::WidgetMap const &AbstractView::getSlots () { return impl->slotWidgetsMap; }

/*--------------------------------------------------------------------------*/

GtkWidget *AbstractView::getSlot (std::string const &name)
{
        AbstractView::WidgetMap::const_iterator i = impl->slotWidgetsMap.find (name);

        if (i == impl->slotWidgetsMap.end ()) {
                return nullptr;
        }

        return i->second;
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

void AbstractView::printStructure ()
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
        gtk_container_foreach (GTK_CONTAINER (mainWidget), &AbstractView::Impl::onPrintWidget, &indent);
}

/*--------------------------------------------------------------------------*/

void AbstractView::Impl::onPrintWidget (GtkWidget *widget, gpointer data)
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
                gtk_container_foreach (GTK_CONTAINER (widget), &AbstractView::Impl::onPrintWidget, &newIndent);
        }
}

/*****************************************************************************/

MappingMultiMap AbstractView::getMappingsByInputRange (std::string const &widgetNameRange) const
{
        //        if (impl->mappingsByInputCache) {
        //                return *impl->mappingsByInputCache;
        //        }

        //        impl->mappingsByInputCache = new MappingMultiMap;

        MappingMultiMap ret;

        for (IMapping *mapping : mappings) {
                if (RegexHelper::nameMatches (mapping->getWidget (), widgetNameRange)) {
                        // impl->mappingsByInputCache->insert (std::make_pair (mapping->getWidget (), mapping));
                        ret.insert (std::make_pair (mapping->getWidget (), mapping));
                }
        }

        return ret;

        //        return *impl->mappingsByInputCache;
}

/*--------------------------------------------------------------------------*/

MappingMultiMap AbstractView::getMappingsByModelRange (std::string const &modelRange) const
{
        MappingMultiMap ret;

        for (IMapping *mapping : mappings) {
                if (RegexHelper::nameMatches (mapping->getModel (), modelRange)) {
                        ret.insert (std::make_pair (mapping->getModel (), mapping));
                }
        }

        return ret;
}

/*****************************************************************************/

AbstractController *AbstractView::getController () { return impl->controller; }
void AbstractView::setController (AbstractController *c) { impl->controller = c; }

/*---------------------------------------------------------------------------*/

void AbstractView::setControllerToUi (AbstractController *c)
{
        if (getUi (getName ())) {
                g_object_set_data (getUi (getName ()), CONTROLLER_KEY, c);
        }
}

AbstractController *AbstractView::getControllerFromUi () { return getControllerByWidget (getUiOrThrow (getName ())); }

AbstractController *AbstractView::getControllerByWidget (GObject *widget)
{
        return static_cast<AbstractController *> (g_object_get_data (widget, CONTROLLER_KEY));
}

/*****************************************************************************/

bool AbstractView::reparent (ViewsToOpen::ViewSlot const &vs)
{
        // Reparent..
        AbstractController *controller = getControllerFromUi ();

/*
 * Do zastanowienia się. Gdy FIND_SLOTS_RECURSIVELY jest 1, to po utworzeniu widoku, GtkForms szuka slotu we wszystkich przodkach
 * jego kontrolera.
 *
 * Gdy jednak FIND_SLOTS_RECURSIVELY 0, to szuka tylko w rodzicu.
 */
#define FIND_SLOTS_RECURSIVELY 1

#if FIND_SLOTS_RECURSIVELY
        while
#else
        if
#endif
                ((controller = controller->getParent ())) {

                ViewVector const &parentControllerViews = controller->getViews ();

                if (parentControllerViews.empty ()) {
#if FIND_SLOTS_RECURSIVELY
                        continue;
#else
                        return false;
#endif
                }

                for (AbstractView *parentControllerView : parentControllerViews) {

                        GtkWidget *slot = parentControllerView->getSlot (vs.slot);

                        if (!slot) {
#if FIND_SLOTS_RECURSIVELY
                                continue;
#else
                                return false;
#endif
                        }

                        if (!GTK_IS_CONTAINER (slot)) {
                                throw Core::Exception ("AbstractView::reparent : !GTK_IS_CONTAINER (slot)");
                        }

                        if (GTK_IS_BIN (slot)) {
                                GtkBin *slotWidget = GTK_BIN (slot);
                                GtkWidget *oldChild = gtk_bin_get_child (slotWidget);

                                /*
                                 * Zawartość tego IF spowodowała wiele niejasności. Kiedy góra jest odkomentowana, to
                                 * podczas umieszczania nowego widoku w slocie, jeśli jest on już zajęty, to zostanie
                                 * opróżniony (wiok + jego kontroler).
                                 *
                                 * Natomiast kiedy dół jest odkomentowany, to jeśli slot jest zajęty, to zostanie na
                                 * konsoli zgłoszony warning i nic więcej się nie stanie.
                                 */
                                if (oldChild) {
#if 1
                                        gtk_widget_hide (oldChild);
                                        gtk_container_remove (GTK_CONTAINER (slotWidget), oldChild);

                                        AbstractController *c = AbstractView::getControllerByWidget (G_OBJECT (oldChild));

                                        if (!c) {
                                                throw Core::Exception ("Could not find controller assigned to thos GtkWidget");
                                        }

                                        c->closeThis ();

#else
                                        BOOST_LOG (lg) << "Cannot reparent view named : [" << getName () << "] into slot named : [" << slotName
                                                       << "], because slot is not empty."
                                                       << "gtk_widget_get_name of the slot : [" << gtk_widget_get_name (slot)
                                                       << "],gtk_buildable_get_name of the slot : [" << gtk_buildable_get_name (GTK_BUILDABLE (slot)) << "],"
                                                       << "gtk_widget_get_name of its child : [" << gtk_widget_get_name (oldChild)
                                                       << "], gtk_buildable_get_name of its child : [" << gtk_buildable_get_name (GTK_BUILDABLE (oldChild))
                                                       << "].";
#endif
                                }
                        }

                        /*---------------------------------------------------------------------------*/
                        /* Feature                                                                   */
                        /*---------------------------------------------------------------------------*/
                        if (!vs.slotFeature.empty ()) {
                                if (GTK_IS_NOTEBOOK (slot) && vs.slotFeature == "label") {
                                        GtkNotebook *notebook = GTK_NOTEBOOK (slot);
                                        gint pageNum = gtk_notebook_get_n_pages (notebook);
                                        GtkWidget *pageChild = gtk_notebook_get_nth_page (notebook, pageNum - 1);

                                        if (!pageChild) {
                                                throw Core::Exception ("AbstractView::reparent : gtk_notebook_get_nth_page returned NULL (no sych page)");
                                        }

                                        gtk_notebook_set_tab_label (notebook, pageChild, GTK_WIDGET (getUi ()));
                                }
                        }
                        else {

                                // Add new.
                                //                        GtkWidget *oldParent = 0;
                                GtkContainer *slotWidget = GTK_CONTAINER (slot);
                                //                        if ((oldParent = gtk_widget_get_parent (GTK_WIDGET (getUi ())))) {
                                //                                gtk_widget_reparent (GTK_WIDGET (getUi ()), GTK_WIDGET (slotWidget));

                                //                                // g_object_ref (tileWidget);
                                //                                // gtk_container_remove (GTK_CONTAINER (oldSlotWidget???), tileWidget);
                                //                                // gtk_container_add (GTK_CONTAINER (slotWidget), tileWidget);
                                //                                // g_object_unref (tileWidget);
                                //                        }
                                //                        else {
                                runDecorators (IPageDecorator::PRE_REPARENT, &controller->getApp ()->getContext (), slotWidget);
                                gtk_container_add (slotWidget, GTK_WIDGET (getUi ()));
                                runDecorators (IPageDecorator::POST_REPARENT, &controller->getApp ()->getContext (), slotWidget);
                                //                        }
                        }

                        BOOST_LOG (lg) << "Reparented view named : [" << getName () << "] into slot named : [" << vs.slot << "], tileWidget ["
                                       << (void *)getUi () << /*"], to slot [" << (void *)slotWidget <<*/ "]";
                }
        }

        return true;
}

/*****************************************************************************/

void AbstractView::setConfig (Config const *c) { impl->config = c; }

/*****************************************************************************/

void AbstractView::clearInternalState ()
{
        impl->inputWidgetsMap.clear ();
        impl->outputWidgetsMap.clear ();
        impl->slotWidgetsMap.clear ();
        //        delete impl->mappingsByInputCache;
        //        impl->mappingsByInputCache = nullptr;
}

/*****************************************************************************/

void AbstractView::runDecorators (IPageDecorator::Stage stage, Context *ctx, GtkContainer *slotWidget)
{
        for (IPageDecorator *decorator : decorators) {
                switch (stage) {
                case IPageDecorator::PRE_SUBMIT:
                        decorator->preSubmit (this, ctx);
                        break;

                case IPageDecorator::PRE_SHOW:
                        decorator->preShow (this, ctx);
                        break;

                case IPageDecorator::POST_SHOW:
                        decorator->postShow (this, ctx);
                        break;

                case IPageDecorator::POST_REFRESH:
                        decorator->postRefresh (this, ctx);
                        break;

                case IPageDecorator::PRE_CLOSE:
                        decorator->preClose (this, ctx);
                        break;

                case IPageDecorator::PRE_REPARENT:
                        decorator->preReparent (this, slotWidget, ctx);
                        break;

                case IPageDecorator::POST_REPARENT:
                        decorator->postReparent (this, slotWidget, ctx);
                        break;

                default:
                        throw Core::Exception ("AbstractView::runDecorators : Hey, programmer, you forgot to add an enum value to this switch!");
                }
        }
}

bool AbstractView::isDeleteOnClose () const { return impl->deleteOnClose; }
void AbstractView::setDeleteOnClose (bool b) { impl->deleteOnClose = b; }

} // namespace GtkForms
