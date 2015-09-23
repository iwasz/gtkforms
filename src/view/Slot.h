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
        REFLECTION_CONSTRUCTOR_ (void)
        Slot () : tile (nullptr) {}

        std::string getName () const { return name; }
        GtkTile *getTile () const { return tile; }

private:

        std::string REFLECTION_FIELD_REFERENCE_INPLACE (name);
        GtkTile *REFLECTION_FIELD_VALUE_INPLACE (tile);

        REFLECTION_END (Slot)
};

typedef std::vector <Slot *> SlotVector;
REFLECTION_COLLECTION (SlotVector)

} /* namespace GtkForms */
#endif /* TILE_H_ */
