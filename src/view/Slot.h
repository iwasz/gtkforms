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
#include "ReflectionMacros.h"

namespace GtkForms {

/**
 * Single definition which tells how to nest one view inside another.
 */
class Slot {
public:
        ctr__ (void)

        std::string getName () const { return name; }
        std::string getTileName () const { return tileName; }

private:

        std::string prr_ (name);
        std::string prr_ (tileName);

        end_ (Slot)
};

typedef std::vector <Slot *> SlotVector;
col_ (SlotVector)

} /* namespace GtkForms */
#endif /* TILE_H_ */
