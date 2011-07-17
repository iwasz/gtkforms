/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKBUILDERFACTORY_H_
#define GTKBUILDERFACTORY_H_

#include <ApiMacro.h>
#include <Reflection.h>
#include <string>
#include <K202.h>
#include <beanWrapper/BeanWrapper.h>
#include "IUIFactory.h"
#include "GtkBuilderAdapter.h"
#include "WidgetAdapter.h"

namespace k202 {
class K202;
}

namespace GtkForms {

/**
 * Odpowiada jednemu (?) oknu GTK ładowanemu z pliku *.ui.
 */
class TILIAE_API ViewAdapter : public IUIFactory {
public:
        __c (void)

        virtual ~ViewAdapter () {}
        _m (create) virtual GtkWidget *create ();

/*--------------------------------------------------------------------------*/

        /**
         * Gets widget property in Core::Variant form. Parametr name should be a
         * valid path with two segments separated with "." (dot). First segment
         * is a name of widget, second one is a property you want to get. Name
         * of a widget will be passed to GtkBuilder.
         */
        _m (get) virtual Core::Variant get (const std::string &name) const;
        Ptr <WidgetAdapter> getWidget (const std::string &name) const;

/*--------------------------------------------------------------------------*/

        std::string getFileName () const { return fileName; }
        _m (setFileName) void setFileName (std::string const &f) { fileName = f; }

        std::string getWidgetName () const { return fileName; }
        _m (setWidgetName) void setWidgetName (std::string const &s) { widgetName = s; }

        Ptr <GtkBuilderAdapter> getGtkBuilderAdapter () const { return gtkBuilderAdapter; }
        _s (setGtkBuilderAdapter) void setGtkBuilderAdapter (Ptr <GtkBuilderAdapter> a) { gtkBuilderAdapter = a; }

        /**
         * Map which contains all surrent elements of MVC structure ie. view, controller and
         * (not necessarily) a model.
         */
        Ptr <Core::VariantMap> getContext () const { return context; }
        _s (setContext) void setContext (Ptr <Core::VariantMap> c) { context = c; }

        /**
         * K202 script language interpretter, which is used to execute signal
         * handlers from views. If none provided, K202::instance will be used.
         */
        Ptr <k202::K202> getK202 () const { return k202; }
        _s (setK202) void setK202 (Ptr <k202::K202> k) { k202 = k; }

        /**
         * Bean wrapper which will be used for transfering model properties
         * to view properties and back. If none is set, BeanWrapper::create
         * will be used.
         */
        Ptr<Wrapper::BeanWrapper> getBeanWrapper () const { return beanWrapper; }
        _s (setBeanWrapper) void setBeanWrapper (Ptr<Wrapper::BeanWrapper> beanWrapper) { this->beanWrapper = beanWrapper; }

/*--------------------------------------------------------------------------*/

private:

        Wrapper::BeanWrapper *myBeanWrapper () const;
        k202::K202 *myK202Script () const;

private:

        std::string fileName;
        std::string widgetName;
        Ptr <GtkBuilderAdapter> gtkBuilderAdapter;
        Ptr <Core::VariantMap> context;
        Ptr <k202::K202> k202;
        mutable Ptr <Wrapper::BeanWrapper> beanWrapper;

        _e (ViewAdapter)
};

/**
 *
 */
struct TILIAE_API ViewException : public Core::Exception {
        ViewException (std::string const &m = "") : Core::Exception (m) {}
        virtual ~ViewException () throw () {}
};

}

#	endif /* GTKBUILDERFACTORY_H_ */
