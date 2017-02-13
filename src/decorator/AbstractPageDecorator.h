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

        virtual void postStart (AbstractView *view, Context *ctx) {}
        virtual void preSubmit (AbstractView *view, Context *ctx) {}
        virtual void postRefresh (AbstractView *view, Context *ctx) {}
};

} /* namespace GtkForms */

#endif // ABSTRACTDECORATOR_H
