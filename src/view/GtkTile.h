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
#include <ReflectionParserAnnotation.h>
#include "GtkAbstractView.h"

namespace GtkForms {
class Context;

/**
 * GUI element which is embeddable in GtkView.
 */
class __tiliae_reflect__ GtkTile : public GtkAbstractView {
public:
        virtual ~GtkTile () {}
};

typedef __tiliae_reflect__ std::map <std::string, GtkTile *> GtkTileMap;

} /* namespace GtkForms */
#endif /* GTKTILE_H_ */
