/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_TILE_H_
#define GTK_FORMS_TILE_H_

#include <string>
#include "ReflectionMacros.h"

namespace GtkForms {

/**
 * Single definition which tells how to nest one view inside another.
 */
class Tile {
public:
        ctr__ (void)

        std::string getSlotWidget () const { return slotWidget; }
        std::string getPlugWidget () const { return plugWidget; }

private:

        std::string prr_ (slotWidget);
        std::string prr_ (plugWidget);

        end_ (Tile)
};

typedef std::vector <Tile *> TileVector;
col_ (TileVector)

} /* namespace GtkForms */
#endif /* TILE_H_ */
