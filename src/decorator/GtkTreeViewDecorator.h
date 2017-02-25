/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef GTKTREEVIEWDECORATOR_H
#define GTKTREEVIEWDECORATOR_H

#include "AbstractPageDecorator.h"
#include <string>

namespace GtkForms {

class __tiliae_reflect__ GtkTreeViewDecorator : public AbstractPageDecorator {
public:
        virtual ~GtkTreeViewDecorator () {}
        virtual void postShow (AbstractView *view, Context *ctx);
        virtual void postRefresh (AbstractView *view, Context *ctx);

        std::string getWidget () const { return widget; }
        void setWidget (const std::string &value) { widget = value; }

        bool postRefreshExpandAll = false;
        bool postStartExpandAll = false;

private:
        void expandAll (AbstractView *view);
        std::string widget;
};

} // GtkForms

#endif // GTKTREEVIEWDECORATOR_H
