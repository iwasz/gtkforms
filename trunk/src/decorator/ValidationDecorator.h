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
#include "ReflectionMacros.h"

namespace GtkForms {

class ValidationDecorator : public IPageDecorator {
public:
        ctr__ (void)
        virtual ~ValidationDecorator () {}
        virtual void run (Page *page, Context *ctx);
        end_ (ValidationDecorator)
};

} /* namespace GtkForms */

#endif /* VALIDATIONDECORATOR_H_ */
