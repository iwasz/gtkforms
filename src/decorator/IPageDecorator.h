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
#include "ReflectionMacros.h"

namespace GtkForms {
class Context;
class Page;

/**
 * Some custom actions to be performed on a page/view.
 */
class IPageDecorator : public Core::Object {
public:
        virtual ~IPageDecorator () {}
        virtual void run (Page *page, Context *ctx) = 0;
};

typedef std::vector <IPageDecorator *> PageDecoratorVector;
col_ (PageDecoratorVector)

} /* namespace GtkForms */

#endif /* IPAGEDECORATOR_H_ */