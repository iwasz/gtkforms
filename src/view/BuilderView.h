/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef BUILDERVIEW_H
#define BUILDERVIEW_H

#include "AbstractView.h"

namespace GtkForms {
class AbstractAccessor;

class __tiliae_reflect__ BuilderView : public AbstractView {
public:

        BuilderView ();
        virtual ~BuilderView ();

        virtual void loadUi (App *app);
        virtual bool isLoaded () const;
        virtual void destroyUi ();
        virtual GObject *getUi () __tiliae_no_reflect__;
        virtual GObject *getUi (std::string const &name) __tiliae_no_reflect__;
        GObject *getUiOrThrow (std::string const &name);
        virtual void connectSignals (AbstractAccessor *accessor);
        virtual void connectSignal (gpointer object, std::string const &signalName, std::string const &code);

public:

        /**
         * @brief Name of an GtkBuilder ui file to load.
         */
        std::string file;
        /**
         * @brief ID of objects to load among the main one (name).
         * Useful for loading things sych as ListStore, Adjustment etc. Has no effect when
         * loadWholeFile was set to true, beacuse whole contents of a file will be loaded then.
         */
        std::string alsoLoad;

        /**
         * @brief Load the whole contents of an ui file to the memory. Useful if you have one
         * file per window.
         */
        bool loadWholeFile;

private:

        struct Impl;
        Impl *impl;
};

}

#endif // BUILDERVIEW_H
