/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKVIEW_H_
#define GTKVIEW_H_

#include <map>
#include <string>
#include <ReflectionParserAnnotation.h>
#include "UiFile.h"
#include "AbstractView.h"

namespace GtkForms {
class Context;

/**
 * View (a top level windows, or some inner GtkWidget) created from GtkBuilder. Thise views are
 * loaded (i.e. memory is alloceted) in GtkView::show, and unloaded (memory is freed) in GtkView::hide.
 */
class __tiliae_reflect__ GtkView : public AbstractView {
public:

        GtkView ();
        virtual ~GtkView ();

        void show ();
        virtual void loadUi (App *app);

private:

        struct Impl;
        Impl *impl;
};

} // namespace GtkForms

#endif /* GTKVIEW_H_ */
