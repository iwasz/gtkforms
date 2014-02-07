/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <gtk/gtk.h>
#include "GtkTileManager.h"
#include "Tile.h"
#include "GtkView.h"
#include "Context.h"

namespace GtkForms {
using namespace std;

/*
 * Excerpt from GTK+ documentation : "A GtkBuilder holds a reference to all objects that it has constructed and drops
 * these references when it is finalized. This finalization can cause the destruction of non-widget objects or widgets
 * which are not contained in a toplevel window. For toplevel windows constructed by a builder, it is the responsibility
 * of the user to call gtk_widget_destroy() to get rid of them and all the widgets they contain."
 *
 * So GtkWidnows should be destroyed explicitely, the others are reference-counted.
 */
void GtkTileManager::reparent (PageOperationResult const &poResult, Context *context, bool show)
{
        map <string, GtkBin *> slots;
        map <string, GtkWidget *> plugs;
        TileVector allTiles;

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
         *   - loadUi (). załaduj UI. jezeli już załadowane, to nic się nie dzieje. To jest pomyślane jako singleton, który zajmuje pamięć lub zwalnia, ale jest tylko jeden.
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
         * - View ma getWidget (bae arg) - zwraca widget o nazwie takiej jak name (w postaci GObject)
         * - View ma get slots (map <string, GtkBin *>)
         * - mamy wszystikie widoki i do zamknięcia i do otworzenia i te już widoczne.
         * - Te do otworzenia ładujemy do pamięci. Stan #1 : Wszystko co jest potrzebne jest w pamięci, każdy widget jest dostępny.

         * - Pobieramy sloty z tych już widocznych i tych załadowanych (do otworzenia).
         * - jesteśmy w stanie #2, w kŧórym wszystko jest w pamięci, mamy wskaźniki do wszystkich slotów wszystkich widoków i wsakźniki do wszystkich widoków (plugów i nie plugów poprzez GObject *view.getWidget ()).
         *
         * - Plugi zawsze mają jakiegos parenta, więc trzeba je reparentować (zawsze).
         *
         */

        // 1. Get all slots and plugs from all views and validate
        for (ViewMap::value_type const &entry : poResult.added) {
                GtkView *view = dynamic_cast <GtkView *> (entry.second);

                if (!view) {
                        throw Core::Exception ("Could not cast one of the views to GtkView. GtkTileManager can operate GtkViews only.");
                }

                // Load the view.
                view->load (context);

                TileVector const &tiles = view->getTiles ();
                copy (tiles.begin (), tiles.end (), back_inserter (allTiles));

                for (Tile *tile : tiles) {
                        if (slots.find (tile->getSlotWidget ()) != slots.end ()) {
                                throw Core::Exception ("There are either two Tiles with the same slot-name : [" + tile->getSlotWidget () + "], or two widgets with this same name.");
                        }

                        // Throws if not found.
                        GObject *obj = view->getGObject (tile->getSlotWidget ());

                        if (obj) {
                                if (!GTK_IS_BIN (obj)) {
                                        throw Core::Exception ("Slots have to be of type GtkBin. Your slot [" + tile->getSlotWidget () + "] is not a GtkBin.");
                                }

                                slots[tile->getSlotWidget ()] = GTK_BIN (obj);
                        }

                        if (plugs.find (tile->getPlugWidget ()) != plugs.end ()) {
                                throw Core::Exception ("There are either two Tiles with the same plug-name : [" + tile->getPlugWidget () + "], or two widgets with this same name.");
                        }

                        obj = view->getGObject (tile->getPlugWidget ());

                        if (obj) {
                                if (!GTK_IS_WIDGET (obj)) {
                                        throw Core::Exception ("Plugs have to be of type GtkWidget. Your plug [" + tile->getPlugWidget () + "] is not a GtkWidget.");
                                }

                                plugs[tile->getPlugWidget ()] = GTK_WIDGET (obj);
                        }
                }
        }

        // 2. Reparent.
        for (Tile *tile : allTiles) {
                GtkBin *slot = 0;
                GtkWidget *plug = 0;

                auto i = slots.find (tile->getSlotWidget ());

                if (i != slots.end ()) {
                        slot = i->second;
                }
                else {
                        continue;
                }

                auto j = plugs.find (tile->getPlugWidget ());

                if (j != plugs.end ()) {
                        plug = j->second;
                }
                else {
                        continue;
                }

                // Throw away old child ... ???
                GtkWidget *oldChild = gtk_bin_get_child (slot);

                if (oldChild) {
                        gtk_container_remove (GTK_CONTAINER (slot), oldChild);
                }

                // Add new.
                GtkWidget *oldParent = 0;
                if ((oldParent = gtk_widget_get_parent (plug))) {
                        gtk_widget_reparent (plug, GTK_WIDGET (slot));
                }
                else {
                        gtk_container_add (GTK_CONTAINER (slot), plug);
                }
        }


        if (show) {
                for (ViewMap::value_type const &entry : poResult.added) {
                        GtkView *view = static_cast <GtkView *> (entry.second);
                        view->show ();
                }
        }
}

} /* namespace GtkForms */
