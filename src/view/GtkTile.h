/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_GTKTILE_H_
#define GTK_FORMS_GTKTILE_H_

#include <gtk/gtk.h>
#include <string>
#include <map>
#include "ReflectionMacros.h"
#include "GtkAbstractView.h"

namespace GtkForms {
class Context;

/**
 * GUI element which is embeddable in GtkView.
 */
class GtkTile : public GtkAbstractView {
public:
        REFLECTION_CONSTRUCTOR_ (void)
        REFLECTION_BASE_CLASS ("GtkAbstractView")

        virtual ~GtkTile () {}

        REFLECTION_END (GtkTile)
};

typedef std::map <std::string, GtkTile *> GtkTileMap;
REFLECTION_COLLECTION (GtkTileMap)

} /* namespace GtkForms */
#endif /* GTKTILE_H_ */
