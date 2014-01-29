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

namespace GtkForms {
using namespace std;


void GtkTileManager::reparent (ViewMap *viewMap, bool show)
{
        map <string, GtkBin *> slots;

        for (ViewMap::value_type const &entry : *viewMap) {
                GtkView *view = dynamic_cast <GtkView *> (entry.second);

                if (!view) {
                        throw Core::Exception ("Could not cast one of the views to GtkView. GtkTileManager can operate GtkViews only.");
                }

                // Load the view.
                view->load ();

                TileVector const &tiles = view->getTiles ();

                // 1. Get all slots from all views.
                for (Tile *tile : tiles) {
                        if (slots.find (tile->getSlotWidget ()) != slots.end ()) {
                                throw Core::Exception ("There are either two Tiles with the same slot-name : [" + tile->getSlotWidget () + "], or two widgets with this same name.");
                        }

                        // Throws if not found.
                        GObject *obj = view->getGObject (tile->getSlotWidget ());

                        if (!G_IS_GTK_BIN (obj)) {
                                throw Core::Exception ("");
                        }

                        slots[tile->getSlotWidget ()] = GTK_BIN (obj);
                }

        }

        if (show) {
//                view->show ();
        }
}

} /* namespace GtkForms */
