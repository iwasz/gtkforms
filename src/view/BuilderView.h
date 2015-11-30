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

public:

        std::string file;

private:

        void connectSignals (AbstractAccessor *accessor);

        struct Impl;
        Impl *impl;
};

}

#endif // BUILDERVIEW_H
