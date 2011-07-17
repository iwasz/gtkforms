/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef WIDGETADAPTER_H_
#define WIDGETADAPTER_H_

#include <Reflection.h>
#include <string>
#include <gtk/gtk.h>
#include <ApiMacro.h>

namespace GtkForms {

/**
 * Wrapper over GtkWidget *, which enables BeanWrapper interoperation.
 * agt and set methods can get and set widget Properties.
 */
class TILIAE_API WidgetAdapter {
public:
        __c (void)
        WidgetAdapter (GtkWidget *w = NULL) : widget (w) {}
        virtual ~WidgetAdapter () {}

        _m (get) virtual Core::Variant get (const std::string &name) const;
        _m (set) virtual void set (const std::string &name, const Core::Variant &object);

/*--------------------------------------------------------------------------*/

        GtkWidget *getWidget () const { return widget; }
        void setWidget (GtkWidget *widget) { this->widget = widget; }

private:

        GtkWidget *widget;
        _e (WidgetAdapter)
};

}

#	endif /* WIDGETADAPTER_H_ */
