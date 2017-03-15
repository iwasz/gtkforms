/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_APP_H_
#define GTK_FORMS_APP_H_

#include "Config.h"
#include "controller/AbstractController.h"
#include "controller/RefreshEvent.h"
#include <ReflectionParserAnnotation.h>
#include <beanWrapper/BeanWrapper.h>
#include <core/Typedefs.h>
#include <gtk/gtk.h>
#include <k202/K202.h>
#include <memory>
#include <string>

namespace GtkForms {

class Context;
struct SubmitEvent;
class RefreshEvent;
class AbstractView;

/**
 * Main object (singleton) of a GtkForms application. It can perofrm operations on main aspects
 * of the app, most notably it manages Controllers and Views.
 */
class __tiliae_reflect__ App {
public:
        virtual void init (std::string const &configurationFile, std::string const &initialControllerName) __tiliae_no_reflect__;

        virtual ~App ();

        /**
         * @brief Starts a controller.
         * Retrieves an instances of controlers with names specified in childControllerNames, and runs it.
         * @param requestor Parent of new controllers which requested their creation.
         * @param childControllerNames
         */
        void open (AbstractController *requestor, Core::StringVector const &childControllerNames) __tiliae_no_reflect__;
        void open (AbstractController *requestor, std::string const &childControllerName) __tiliae_no_reflect__;
        void close (AbstractController *requestor, Core::StringVector const &controllerNames = Core::StringVector ()) __tiliae_no_reflect__;
        void close (AbstractController *requestor, std::string const &controllerName) __tiliae_no_reflect__;

        /**
         * Quits entire application regardless of controller nesting level. Programmer
         * (user of the library) invokes this to end the app.
         */
        void quit ();

        /**
         * Handles quit request from GUI user (i.e. when he clicks widow manager's close button). Can be invoked
         * programmaticaly by the programmer as well, for example as a handler of "quit" menu action.
         */
        void userQuitRequest ();

        /**
         *
         */
        void show (std::string const &page) {}
        void hide (std::string const &page) {}

        Context &getContext ();
        static k202::K202 *getK202 ();

        /// BeanWrapper used for view to model mapping. Not used in the container.
        Wrapper::BeanWrapper *getBeanWrapper ();

        Config const *getConfig () const;

protected:
        /// Initialization of BeanWrapper used for view to model mapping. Not used in the container. Add custom editors here.
        virtual void initBeanWrapper ();

        /// Initialization of the container. Add custom conversions here.
        virtual std::unique_ptr<Container::BeanFactoryContainer> createContainer (Ptr<Container::MetaContainer> metaContainer);

private:
        /**
         * Deals with SubmitEvents.
         */
        void doSubmit (SubmitEvent *event);
        void doRefresh (RefreshEvent *event);

        /**
         * Creates a tiliae container instance (pointer is in Impl).
         */
        void initContainer (std::string const &configFile);

        /**
         * Method which has to be invoked in your main application loop.
         */
        void run ();
        void manageControllers ();
        void pushEvent (std::unique_ptr<IEvent> e) __tiliae_no_reflect__;

        friend struct SubmitEvent;
        friend class RefreshEvent;
        friend class QuitEvent;
        friend void AbstractController::refresh (std::string const &widgetNameRange, std::string const &propertyName);
        friend void AbstractController::submit (const std::string &widgetNameRange, const std::string &controllerName);
        friend gboolean guiThread (gpointer user_data);

private:
        struct Impl;
        Impl *impl;
};

} // namespace GtkForms

#endif /* APP_H_ */
