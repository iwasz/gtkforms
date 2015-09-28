/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef VALIDATIONDECORATOR_H_
#define VALIDATIONDECORATOR_H_

#include "IPageDecorator.h"
#include <ReflectionParserAnnotation.h>

namespace GtkForms {

class __tiliae_reflect__ ValidationDecorator : public IPageDecorator {
public:
        REFLECTION_CONSTRUCTOR_ (void)
        virtual ~ValidationDecorator () {}
        virtual void run (Page *page, Context *ctx);
        REFLECTION_END (ValidationDecorator)
};

} /* namespace GtkForms */

#endif /* VALIDATIONDECORATOR_H_ */
