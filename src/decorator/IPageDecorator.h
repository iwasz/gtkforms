/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef IPAGEDECORATOR_H_
#define IPAGEDECORATOR_H_

#include <ReflectionParserAnnotation.h>
#include <Tiliae.h>

namespace GtkForms {
class Context;
class AbstractView;

/**
 * Some custom actions to be performed on a page/view.
 */
class __tiliae_reflect__ IPageDecorator : public Core::Object {
public:
        enum Stage { PRE_SHOW, POST_SHOW, PRE_SUBMIT, POST_REFRESH, PRE_CLOSE };

        virtual ~IPageDecorator () {}
        /// On view start
        virtual void preShow (AbstractView *view, Context *ctx) = 0;
        /// On view start
        virtual void postShow (AbstractView *view, Context *ctx) = 0;
        /// On submit (prior to controller->onSubmit).
        virtual void preSubmit (AbstractView *view, Context *ctx) = 0;
        /// After refresh (after widgets are populated with data).
        virtual void postRefresh (AbstractView *view, Context *ctx) = 0;
        /// Before close (does not matter if window gets destroyed or not)
        virtual void preClose (AbstractView *view, Context *ctx) = 0;
};

typedef __tiliae_reflect__ std::vector<IPageDecorator *> PageDecoratorVector;

} /* namespace GtkForms */

#endif /* IPAGEDECORATOR_H_ */
