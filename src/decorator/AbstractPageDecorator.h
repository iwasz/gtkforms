/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ABSTRACTDECORATOR_H
#define ABSTRACTDECORATOR_H

#include "IPageDecorator.h"
#include <ReflectionParserAnnotation.h>

namespace GtkForms {

class __tiliae_reflect__ AbstractPageDecorator : public IPageDecorator {
public:
        virtual ~AbstractPageDecorator () {}

        virtual void preShow (AbstractView *view, Context *ctx) {}
        virtual void postShow (AbstractView *view, Context *ctx) {}
        virtual void preSubmit (AbstractView *view, Context *ctx) {}
        virtual void postRefresh (AbstractView *view, Context *ctx) {}
        virtual void preClose (AbstractView *view, Context *ctx) {}
        virtual void preReparent (AbstractView *view, GtkContainer *slotWidget, Context *ctx) {}
        virtual void postReparent (AbstractView *view, GtkContainer *slotWidget, Context *ctx) {}
};

} /* namespace GtkForms */

#endif // ABSTRACTDECORATOR_H
