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
        REFLECTION_CONSTRUCTOR_ (void)
        REFLECTION_BASE_CLASS ("GtkAbstractView")

        GtkView ();
        virtual ~GtkView ();

        void show ();
        virtual void loadUi (App *app);

private:

        class Impl;
        Impl *impl;

        REFLECTION_END (GtkView)
};

} // namespace GtkForms

#endif /* GTKVIEW_H_ */
