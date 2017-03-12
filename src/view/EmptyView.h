/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef EMPTYVIEW_H
#define EMPTYVIEW_H

#include "AbstractView.h"

namespace GtkForms {
class AbstractAccessor;

class __tiliae_reflect__ EmptyView : public AbstractView {
public:
        virtual ~EmptyView () {}

        void loadUi (App *app) {}
        bool isLoaded () const { return true; }
        void destroyUi () {}
        GObject *getUi () __tiliae_no_reflect__ { return nullptr; }
        GObject *getUi (std::string const &name) __tiliae_no_reflect__ { return nullptr; }
        void connectSignals (AbstractAccessor *accessor) {}
};
}

#endif // EMPTYVIEW_H
