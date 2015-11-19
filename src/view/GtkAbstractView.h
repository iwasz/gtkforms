/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_ABSTRACT_VIEW_H_
#define GTK_ABSTRACT_VIEW_H_

#include "IView.h"
#include <ReflectionParserAnnotation.h>
#include "UiFile.h"

namespace GtkForms {
class Context;

/**
 * View (a top level windows, or some inner GtkWidget) created from GtkBuilder. Thise views are
 * loaded (i.e. memory is alloceted) in GtkAbstractView::show, and unloaded (memory is freed) in GtkAbstractView::hide (loadUi/destroyUi?).
 */
class __tiliae_reflect__  GtkAbstractView : public IView {
public:

        GtkAbstractView ();
        virtual ~GtkAbstractView ();

        std::string getName () const { return name; }

        /**
         * Loads a UI file into memory (if not loaded allready), and gets pointer to widget with name
         * name. If widget was obtained from the file earlier, it will be returned immediately, and no
         * new instance of this widget will be made.
         */
        virtual void loadUi (App *app);
        virtual void show ();
        virtual void hide ();
        virtual void refresh (Context *) {}

        /**
         * Destroys
         */
        virtual void destroyUi ();

        /**
         * Gets the main object (GTK+ GObject) from the ui file.
         */
        virtual GObject *getUi () __tiliae_no_reflect__;
        virtual bool isLoaded () const;

        /**
         * Get arbitrary object from UI.
         */
        virtual GObject *getUiOrThrow (std::string const &name);
        virtual GObject *getUi (std::string const &name) __tiliae_no_reflect__;

        InputMap getInputs (std::string const &dataRange, bool outputs = false);
        void printStructure ();
        void contId (std::string const &id) { name = id; }

        std::string name;
        std::string ui;

private:

        void populateInputMap ();

        UiFile *uiFile;

        struct Impl;
        Impl *impl = 0;
};

} // namespace GtkForms

#endif /* GTKVIEW_H_ */
