/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#if 0
#ifndef GTK_FORMS_SLOT_H_
#define GTK_FORMS_SLOT_H_

#include <string>
#include "GtkTile.h"
#include <ReflectionParserAnnotation.h>
#include <vector>

namespace GtkForms {

/**
 * Single definition which tells how to nest one view inside another.
 */
class __tiliae_reflect__ Slot {
public:
        Slot () : tile (nullptr) {}

        std::string getName () const { return name; }
        GtkTile *getTile () const { return tile; }

        std::string name;
        GtkTile *tile;
};

typedef __tiliae_reflect__ std::vector <Slot *> SlotVector;

} /* namespace GtkForms */
#endif /* TILE_H_ */
#endif
