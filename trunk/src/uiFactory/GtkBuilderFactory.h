/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKBUILDERFACTORY_H_
#define GTKBUILDERFACTORY_H_

#include <core/ApiMacro.h>
#include <reflection/Reflection.h>
#include <string>
#include "IUIFactory.h"
#include "GtkBuilderAdapter.h"

class TILIAE_API GtkBuilderFactory : public IUIFactory {
public:
        __c (void)

        virtual ~GtkBuilderFactory () {}
        _m (create) virtual GtkWidget *create (bool show = false);

/*--------------------------------------------------------------------------*/

        std::string getFileName () const { return fileName; }
        _m (setFileName) void setFileName (std::string const &f) { fileName = f; }

        std::string getWidgetName () const { return fileName; }
        _m (setWidgetName) void setWidgetName (std::string const &s) { widgetName = s; }

        Ptr <GtkBuilderAdapter> getGtkBuilderAdapter () const { return gtkBuilderAdapter; }
        _s (setGtkBuilderAdapter) void setGtkBuilderAdapter (Ptr <GtkBuilderAdapter> a) { gtkBuilderAdapter = a; }

private:

        std::string fileName;
        std::string widgetName;
        Ptr <GtkBuilderAdapter> gtkBuilderAdapter;

        _e (GtkBuilderFactory)
};

#	endif /* GTKBUILDERFACTORY_H_ */
