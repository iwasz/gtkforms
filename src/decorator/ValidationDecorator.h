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

/**
 * @brief Changs color of an input to red if validation falis.
 */
class __tiliae_reflect__ ValidationDecorator : public IPageDecorator {
public:
        virtual ~ValidationDecorator () {}
        virtual void run (AbstractView *view, Context *ctx);
};

} /* namespace GtkForms */

#endif /* VALIDATIONDECORATOR_H_ */
