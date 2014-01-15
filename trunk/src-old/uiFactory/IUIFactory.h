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
#include <core/ApiMacro.h>
#include <core/Object.h>

class TILIAE_API IUIFactory : public Core::Object {
public:
        virtual ~IUIFactory () {}
        virtual GtkWidget *create (bool = false) = 0;
};

#	endif /* IUIFACTORY_H_ */
