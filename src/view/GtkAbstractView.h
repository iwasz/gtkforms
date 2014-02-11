/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_ABSTRACT_VIEW_H_
#define GTK_ABSTRACT_VIEW_H_

#include <gtk/gtk.h>
#include "IView.h"
#include "ReflectionMacros.h"
#include "UiFile.h"

namespace GtkForms {
class Context;

/**
 * View (a top level windows, or some inner GtkWidget) created from GtkBuilder. Thise views are
 * loaded (i.e. memory is alloceted) in GtkAbstractView::show, and unloaded (memory is freed) in GtkAbstractView::hide.
 */
class GtkAbstractView : public IView {
public:
        abt__

        GtkAbstractView ();
        virtual ~GtkAbstractView ();

        std::string getName () const { return name; }

        /**
         * Loads a UI file into memory (if not loaded allready), and gets pointer to widget with name
         * name. If widget was obtained from the file earlier, it will be returned immediately, and no
         * new instance of this widget will be made.
         */
        virtual void loadUi (Context *context);
        virtual void show ();
        virtual void hide ();

        /**
         * Destroys
         */
        virtual void destroyUi ();

        /**
         * Gets the main object (GTK+ GObject) from the ui file.
         */
        virtual GObject *getUi ();

        /**
         * Get arbitrary object from UI.
         */
        virtual GObject *getUi (std::string const &name);

private:

        void connectSignals (Context *context);

private:

        std::string prr_ (name);
        UiFile *prr_ (uiFile);

        class Impl;
        Impl *impl = 0;

        end_ (GtkAbstractView)
};

} // namespace GtkForms

#endif /* GTKVIEW_H_ */
