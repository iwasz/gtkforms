/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Page.h"

#include "view/GtkView.h"
#include "view/Slot.h"
#include "Logging.h"
#include "RegexHelper.h"

namespace GtkForms {
static src::logger_mt &lg = logger::get ();
using namespace std;

Page::~Page ()
{
        delete mappingsByInputCache;
        //        delete mappingsByModelCache;
}

/*--------------------------------------------------------------------------*/

void Page::loadUi (App *app)
{
        if (view) {
                view->loadUi (app);
        }

        for (Slot *s : slots) {
                GtkTile *tile = s->getTile ();

                if (tile) {
                        tile->loadUi (app);
                }
        }
}

/*--------------------------------------------------------------------------*/

GObject *Page::getUiOrThrow (std::string const &name)
{
        GObject *obj = getUi (name);

        if (!obj) {
                throw Core::Exception ("Page::getUiOrThrow could not find ARBITRARY object in UI (mainView + all tiles). Object name : [" + std::string (name)
                                       + "].");
        }

        return obj;
}

/*--------------------------------------------------------------------------*/

GObject *Page::getUi (std::string const &name)
{
        GObject *obj = nullptr;

        if (view) {
                obj = view->getUi (name);

                if (obj) {
                        return obj;
                }
        }

        for (Slot *s : slots) {
                GtkTile *tile = s->getTile ();

                if (tile) {
                        obj = tile->getUi (name);

                        if (obj) {
                                return obj;
                        }
                }
        }

        return 0;
}

/*--------------------------------------------------------------------------*/

void Page::destroyUi ()
{
        for (Slot *s : slots) {
                GtkTile *tile = s->getTile ();
                tile->destroyUi ();
        }

        if (view) {
                view->destroyUi ();
        }
}

/*--------------------------------------------------------------------------*/

void Page::show ()
{
        if (!view) {
                throw Core::Exception ("Page::show : no main view to show.");
        }

        view->show ();
}

/*--------------------------------------------------------------------------*/

void Page::hide ()
{
        if (!view) {
                throw Core::Exception ("Page::hide : no main view to hide.");
        }

        view->hide ();
}

/*--------------------------------------------------------------------------*/

ostream &operator<< (ostream &o, Page const &p)
{
        o << "Page [view : [" << p.view->getName () << "], tiles : [";

        for (auto i = p.slots.begin (); i != p.slots.end ();) {
                GtkTile *tile = (*i)->getTile ();
                o << tile->getName ();

                if (++i != p.slots.end ()) {
                        o << ", ";
                }
        }

        o << "]]";
        return o;
}

/*--------------------------------------------------------------------------*/

MappingMultiMap const &Page::getMappingsByInput () const
{
        if (mappingsByInputCache) {
                return *mappingsByInputCache;
        }

        mappingsByInputCache = new MappingMultiMap;

        for (IMapping *mapping : mappings) {
                mappingsByInputCache->insert (std::make_pair (mapping->getWidget (), mapping));
        }

        return *mappingsByInputCache;
}

/*--------------------------------------------------------------------------*/

MappingMultiMap Page::getMappingsByModelRange (std::string const &modelRange) const
{
        MappingMultiMap ret;

        for (IMapping *mapping : mappings) {
                if (RegexHelper::modelNameMatches (mapping->getModel (), modelRange)) {
                        ret.insert (std::make_pair (mapping->getModel (), mapping));
                }
        }

        return ret;
}

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

} // namespace GtkForms
