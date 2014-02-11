/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKVIEW_H_
#define GTKVIEW_H_

#include <map>
#include <string>
#include "ReflectionMacros.h"
#include "UiFile.h"
#include "GtkAbstractView.h"
#include "GtkTile.h"
#include "Slot.h"

namespace GtkForms {
class Context;

/**
 * View (a top level windows, or some inner GtkWidget) created from GtkBuilder. Thise views are
 * loaded (i.e. memory is alloceted) in GtkView::show, and unloaded (memory is freed) in GtkView::hide.
 */
class GtkView : public GtkAbstractView {
public:
        ctr__ (void)
        bse_ ("GtkAbstractView")

        virtual ~GtkView () {}

//        void loadUi (Context *context);
        void show ();
//        void hide ();
//        void destroy ();

        void model2View (std::string const &dataRange);
        void view2Model (std::string const &dataRange);
        void reparent (GtkTileMap const &tiles, SlotVector const &slots, Context *context);

private:

        typedef std::map <std::string, GtkBin *> SlotWidgetMap;
        SlotWidgetMap getSlotWidgets (SlotVector const &slots);

        end_ (GtkView)
};

} // namespace GtkForms

#endif /* GTKVIEW_H_ */
