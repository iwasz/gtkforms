/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_SLOT_H_
#define GTK_FORMS_SLOT_H_

#include <string>
#include "GtkTile.h"
#include "ReflectionMacros.h"

namespace GtkForms {

/**
 * Single definition which tells how to nest one view inside another.
 */
class Slot {
public:
        ctr__ (void)
        Slot () : tile (nullptr) {}

        std::string getName () const { return name; }
        GtkTile *getTile () const { return tile; }

private:

        std::string prr_ (name);
        GtkTile *prp_ (tile);

        end_ (Slot)
};

typedef std::vector <Slot *> SlotVector;
col_ (SlotVector)

} /* namespace GtkForms */
#endif /* TILE_H_ */
