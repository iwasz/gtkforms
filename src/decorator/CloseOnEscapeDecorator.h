/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef CLOSEONESCAPEDECORATOR_H
#define CLOSEONESCAPEDECORATOR_H

#include "AbstractPageDecorator.h"
#include <gtk/gtk.h>

namespace GtkForms {

class __tiliae_reflect__ CloseOnEscapeDecorator : public AbstractPageDecorator {
public:
        virtual ~CloseOnEscapeDecorator () {}
        virtual void preShow (AbstractView *view, Context *ctx);

private:
        static gboolean onKeyPress (GtkWidget *widget, GdkEvent *event, gpointer userData);
};

} // namespace

#endif // CLOSEONESCAPEDECORATOR_H
