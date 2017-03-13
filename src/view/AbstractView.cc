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

        //        mutable MappingMultiMap *mappingsByInputCache = nullptr;
        AbstractController *controller = nullptr;
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

AbstractView *AbstractView::loadView (ViewsToOpen::ViewSlot const &vs, AbstractController *controller, Ptr<Container::BeanFactoryContainer> container)
{
        AbstractView *view = ocast<AbstractView *> (container->getBean (vs.view));
        // Ustawi w polu impl->controller
        view->setController (controller);
        view->loadUi (controller->getApp ());
        // Teraz ma załadowany widok, i ustawi
        view->setControllerToUi (controller);
        view->connectSignals (controller->getModelAccessor ());

        if (!vs.slot.empty ()) {
                if (!view->reparent (vs.slot)) {
                        // TODO zmienić na warning
                        BOOST_LOG (lg) << "Warn : could not reparent!";
                }
        }

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

bool AbstractView::reparent (std::string const &slotName)
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

                        GtkWidget *slot = parentControllerView->getSlot (slotName);

                        if (!slot) {
#if FIND_SLOTS_RECURSIVELY
                                continue;
#else
                                return false;
#endif
                        }

                        if (!GTK_IS_BIN (slot)) {
                                throw Core::Exception ("AbstractView::reparent : !GTK_IS_BIN (slot)");
                        }

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
                                               << "], gtk_buildable_get_name of its child : [" << gtk_buildable_get_name (GTK_BUILDABLE (oldChild)) << "].";
#endif
                        }

                        // Add new.
                        GtkWidget *oldParent = 0;
                        if ((oldParent = gtk_widget_get_parent (GTK_WIDGET (getUi ())))) {
                                gtk_widget_reparent (GTK_WIDGET (getUi ()), GTK_WIDGET (slotWidget));

                                //                        g_object_ref (tileWidget);
                                //                        gtk_container_remove (GTK_CONTAINER (oldSlotWidget???), tileWidget);
                                //                        gtk_container_add (GTK_CONTAINER (slotWidget), tileWidget);
                                //                        g_object_unref (tileWidget);
                        }
                        else {
                                gtk_container_add (GTK_CONTAINER (slotWidget), GTK_WIDGET (getUi ()));
                        }

                        BOOST_LOG (lg) << "Reparented view named : [" << getName () << "] into slot named : [" << slotName << "], tileWidget ["
                                       << (void *)getUi () << "], to slot [" << (void *)slotWidget << "]";
                }
        }

        return true;
}

#if 0
/*
 * Excerpt from GTK+ documentation : "A GtkBuilder holds a reference to all objects that it has constructed and drops
 * these references when it is finalized. This finalization can cause the destruction of non-widget objects or widgets
 * which are not contained in a toplevel window. For toplevel windows constructed by a builder, it is the responsibility
 * of the user to call gtk_widget_destroy() to get rid of them and all the widgets they contain."
 *
 * So GtkWidnows should be destroyed explicitely, the others are reference-counted.
 */
void Page::reparent (Context *context)
{
        //        map <string, GtkBin *> slots;
        //        map <string, GtkWidget *> tiles;
        //        SlotVector allTiles;

        /*
         * Możliwości:
         * - Otworzyć nowy widok (A) składający się z kafelków.
         *  - Ładujemy główny widok.
         *  - Ładujemy kafelki.
         *  - Składamy do kupy.
         *
         * - Otworzyć jeszcze jeden widok (B) (możliwe że też kafelkowy) nad tym już istniejącym (drugie top-level window).
         *  - Ładujemy nowy widok, nie ruszamy tego starego.
         *  - Ładujemy kafelki.
         *  - Składamy do kupy.
         *
         * - Otworzyć nowy kafelek i dokleić go do już istniejącego widoku.
         *  - Załadować kafelek.
         *  - Umiescić go na miejscu.
         *
         * - Otworzyć widok (C), który ma zamienić widok (A) i (B)
         *  - Ładujemy główny widok.
         *  - Ładujemy kafelki (te kóre nie są załadowane).
         *  - Zamykamy poprzedni główny widok.
         *  - Składamy do kupy.
         *
         * - Page , czyli strona skłąda się tylko:
         *  - GtkWindow - tylko jedno (standalone).
         *  - GtkTile - wiele.
         *  - Z poniższego wynika, że strona musi mieć nazwę, a więc musi być jakaś mapa stron zdefiniowana niestety.
         * Operacje
         *  - Page.getName ().  Pobierz nazwę.
         *  - Page.getView (). Pobierz głowne okno.
         *  - Page.getTiles (). Pobierz kafelki.
         *  loadUi (). Wywołuje loadUi na wsyztskich tilesach i na GtkWindow
         *
         *  GtkTile operacje. Kafelek.
         *   - loadUi (). załaduj UI. jezeli już załadowane, to nic się nie dzieje. To jest pomyślane jako singleton, który zajmuje pamięć lub zwalnia, ale jest
         tylko jeden.
         *   - getUi ().  pobierz ui.
         *   - show ().
         *   - destroy ()
         *
         * GtkView/albo GtkWindow. Główne okno
         *   - loadUi (). załaduj UI.
         *   - getUi ().  pobierz ui.
         *   - show ().
         *   - destroy ()
         *   - reparent (map <string, GtkTile *>).
         *
         *
         *
         *  Wartości zwracane z kontrolerów (nazwy widoków ze specalnymi znacznikami):
         *  +page (otwórz stronę).
         *  -page (zamknij stronę, ale po załadowaniu srona nie ma nazwy, więc jak ją zamknąć?).
         *  pageA->pageB, lub po prostu ->page (move, czyli zmień aktualną stronę na nową stronę przenosząc kafelki jeśli się da)
         *  +pageA,+pageB otwórz dwie strony.
         *
         * Tylko operacja -> wymaga wyjaśnienia. Operacja pageA->pageB.
         * - pageA ma widok głowny (umownie GtkWindow), i pageB też. Ten z A jest NA PEWNO DO ZAMKNIĘCIA, a ten z B do otworzenia (załadować).
         * - kafelki z A są już załadowane. Trzeba załadować kafelki od widoku B.
         * - Pula kafelków to teraz jest pula z A i z B - trzeba je dodac do jednej mapy - jeżeli nazwy się powtarzają, to wygrywają te nowsze,
         *   alternatywnie kafelki muszą mieć unikalne nazwy. Widok docelowy to jest
         * - Umieszczamy kafelki w slotach.
         *
         * Operacja +page.
         * - Container.getBean (Page.getname ()) Znajdujemy page w tej super-mapie (singleton, załadowany już).
         * - Page.loadUi (). ładujemy gowny widok i kafelki.
         * - tiles = Page.getTiles (). Pula kafelków : mapa, albo jakiś obiekt typu mapa.
         * - mainWindow = Page.getMainWindow . pobieramy główny widok.
         * - mainWindow.reparent (tiles);
         * - Page.show (). Pokazujemy wszystko. (lub mainWindow.show ()).
         *
         * ----------
         *
         * - View ma getWidget (bae arg) - zwraca widget o nazwie takiej jak name (w postaci GObject)
         * - View ma get slots (map <string, GtkBin *>)
         * - mamy wszystikie widoki i do zamknięcia i do otworzenia i te już widoczne.
         * - Te do otworzenia ładujemy do pamięci. Stan #1 : Wszystko co jest potrzebne jest w pamięci, każdy widget jest dostępny.

         * - Pobieramy sloty z tych już widocznych i tych załadowanych (do otworzenia).
         * - jesteśmy w stanie #2, w kŧórym wszystko jest w pamięci, mamy wskaźniki do wszystkich slotów wszystkich widoków i wsakźniki do wszystkich widoków
         (plugów i nie plugów poprzez GObject *view.getWidget ()).
         *
         * - Plugi zawsze mają jakiegos parenta, więc trzeba je reparentować (zawsze).
         *
         */

        SlotWidgetMap slotWidgets = getSlotWidgets ();

        // 2. Reparent.
        for (Slot *slot : slots) {
                GtkBin *slotWidget = 0;
                GtkWidget *tileWidget = 0;

                auto i = slotWidgets.find (slot->getName ());

                if (i != slotWidgets.end ()) {
                        slotWidget = i->second;
                }
                else {
                        throw Core::Exception ("No such slot [" + slot->getName () + "]");
                }

                GtkTile *gtkTile = slot->getTile ();

                if (!gtkTile) {
                        throw Core::Exception ("GtkView::reparent : no tile in slot.");
                }

                tileWidget = GTK_WIDGET (gtkTile->getUi ());

                // Throw away old child ... ???
                GtkWidget *oldChild = gtk_bin_get_child (slotWidget);

                if (oldChild) {
                        gtk_container_remove (GTK_CONTAINER (slotWidget), oldChild);
                }

                // Add new.
                GtkWidget *oldParent = 0;
                if ((oldParent = gtk_widget_get_parent (tileWidget))) {
                        gtk_widget_reparent (tileWidget, GTK_WIDGET (slotWidget));

//                        g_object_ref (tileWidget);
//                        gtk_container_remove (GTK_CONTAINER (oldSlotWidget???), tileWidget);
//                        gtk_container_add (GTK_CONTAINER (slotWidget), tileWidget);
//                        g_object_unref (tileWidget);
                }
                else {
                        gtk_container_add (GTK_CONTAINER (slotWidget), tileWidget);
                }

                BOOST_LOG (lg) << "Reparented : tile : [" << (void *)gtkTile << "], tileName : [" << gtkTile->getName () << "], tileWidget ["
                               << (void *)tileWidget << "], to slot [" << (void *)slotWidget << "]";
        }
}

/*--------------------------------------------------------------------------*/

Page::SlotWidgetMap Page::getSlotWidgets ()
{
        SlotWidgetMap slotWidgets;

        for (Slot *slot : slots) {
                if (slotWidgets.find (slot->getName ()) != slotWidgets.end ()) {
                        throw Core::Exception ("There are either two Tiles with the same slot-name : [" + slot->getName ()
                                               + "], or two widgets with this same name.");
                }

                // Throws if not found.
                GObject *obj = view->getUi (slot->getName ());

                if (obj) {
                        addToMapOrThrow (obj, slot, &slotWidgets);
                        continue;
                }

                for (Slot *s : slots) {
                        GtkTile *tile = s->getTile ();
                        GObject *obj = tile->getUi (slot->getName ());

                        if (obj) {
                                addToMapOrThrow (obj, slot, &slotWidgets);
                                break;
                        }
                }
        }

        return slotWidgets;
}

/*--------------------------------------------------------------------------*/

void Page::addToMapOrThrow (GObject *obj, Slot *slot, SlotWidgetMap *slotWidgets)
{
        if (!GTK_IS_BIN (obj)) {
                throw Core::Exception ("Slot widgets have to be of type GtkBin. Your slot [" + slot->getName () + "] is not a GtkBin.");
        }

        BOOST_LOG (lg) << "Found slot : [" << slot->getName () << "]";
        slotWidgets->operator[] (slot->getName ()) = GTK_BIN (obj);
}

#endif

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

void AbstractView::runDecorators (IPageDecorator::Stage stage, Context *ctx)
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

                default:
                        throw Core::Exception ("AbstractView::runDecorators : Hey, programmer, you forgot to add an enum value to this switch!");
                }
        }
}

} // namespace GtkForms
