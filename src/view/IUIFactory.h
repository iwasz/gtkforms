/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef IUIFACTORY_H_
#define IUIFACTORY_H_

#include <gtk/gtk.h>
#include <ApiMacro.h>
#include <Object.h>

namespace GtkForms {

class TILIAE_API IUIFactory : public Core::Object {
public:
        virtual ~IUIFactory () {}
        virtual GtkWidget *create () = 0;
};

}

#	endif /* IUIFACTORY_H_ */
