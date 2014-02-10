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

class GtkTile : public GtkAbstractView {
public:
        ctr__ (void)
        bse_ ("GtkAbstractView")

        virtual ~GtkTile () {}

//        void loadUi (Context *context);
//        void show ();
//        void hide ();
//        void destroy ();
//
//        /**
//         * Gets a object (GTK+ GObject) from the ui file.
//         */
//        GObject *getUi ();

        end_ (GtkTile)
};

typedef std::map <std::string, GtkTile *> GtkTileMap;
col_ (GtkTileMap)

} /* namespace GtkForms */
#endif /* GTKTILE_H_ */
