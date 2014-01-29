/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKVIEW_H_
#define GTKVIEW_H_

#include <gtk/gtk.h>
#include "IView.h"
#include "ReflectionMacros.h"
#include "UiFile.h"
#include "Tile.h"

namespace GtkForms {

/**
 * View (a top level windows, or some inner GtkWidget) created from GtkBuilder. Thise views are
 * loaded (i.e. memory is alloceted) in GtkView::show, and unloaded (memory is freed) in GtkView::hide.
 */
class GtkView : public IView {
public:
        ctr__ (void)

        GtkView ();
        virtual ~GtkView ();

        std::string getName () const { return name; }

        void load ();
        void show ();
        void hide ();
        void destroy ();

        void model2View (std::string const &dataRange);
        void view2Model (std::string const &dataRange);

        TileVector const &getTiles () const { return tiles; }

        /**
         * Gets a object (GTK+ GObject) from the ui file.
         */
        GObject *getGObject (std::string const &name);

private:

        std::string prr_ (name);
        UiFile *prr_ (uiFile);
        TileVector prr_ (tiles);

        class Impl;
        Impl *impl = 0;

        end_ (GtkView)
};

} // namespace GtkForms

#endif /* GTKVIEW_H_ */
