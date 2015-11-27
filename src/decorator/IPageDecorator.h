/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef IPAGEDECORATOR_H_
#define IPAGEDECORATOR_H_

#include <Tiliae.h>
#include <ReflectionParserAnnotation.h>

namespace GtkForms {
class Context;
class AbstractView;

/**
 * Some custom actions to be performed on a page/view.
 */
class __tiliae_reflect__ IPageDecorator : public Core::Object {
public:
        virtual ~IPageDecorator () {}
        virtual void run (AbstractView *view, Context *ctx) = 0;
};

typedef __tiliae_reflect__ std::vector <IPageDecorator *> PageDecoratorVector;

} /* namespace GtkForms */

#endif /* IPAGEDECORATOR_H_ */
