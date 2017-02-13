/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef VALIDATIONDECORATOR_H_
#define VALIDATIONDECORATOR_H_

#include "AbstractPageDecorator.h"

namespace GtkForms {

/**
 * @brief Changs color of an input to red if validation falis.
 */
class __tiliae_reflect__ ValidationDecorator : public AbstractPageDecorator {
public:
        virtual ~ValidationDecorator () {}
        virtual void preSubmit (AbstractView *view, Context *ctx);
};

} /* namespace GtkForms */

#endif /* VALIDATIONDECORATOR_H_ */
