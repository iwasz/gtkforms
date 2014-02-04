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
         * - Otworzyć jeszcze jeden widok (B) (możliwe że też kafelkowy) nad tym już istniejącym (drugie top-level window).
         * -
         *
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
