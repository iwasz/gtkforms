/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_WIDGETADAPTER_H_
#define GTK_FORMS_WIDGETADAPTER_H_

#include <Tiliae.h>
#include <string>
#include <gtk/gtk.h>
#include <core/ApiMacro.h>

namespace GtkForms {

/**
 * Wrapper over GtkWidget *, which enables BeanWrapper interoperation.
 * agt and set methods can get and set widget Properties.
 */
class GObjectAdapter {
public:
//        __c (void)
        GObjectAdapter (GObject *w = NULL) : object (w) {}
        virtual ~GObjectAdapter () {}

        /*_m (get)*/ virtual Core::Variant get (const std::string &name) const;
        /*_m (set)*/ virtual void set (const std::string &name, const Core::Variant &object);

/*--------------------------------------------------------------------------*/

        GObject *getGObject () const { return object; }
        void setGObject (GObject *widget) { this->object = widget; }

private:

        GObject *object;
};

}

#	endif /* WIDGETADAPTER_H_ */
