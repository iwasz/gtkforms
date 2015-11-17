/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_APP_H_
#define GTK_FORMS_APP_H_

#include <string>
#include <ReflectionParserAnnotation.h>
#include <gtk/gtk.h>
#include "controller/RefreshEvent.h"
#include "Config.h"

namespace GtkForms {

class IUnit;
class Page;
class Context;
class SubmitEvent;
class RefreshEvent;

/**
 * Main object (singleton) of a GtkForms application. It can perofrm operations on main aspects
 * of the app, most notably it manages Units and Pages.
 */
class __tiliae_reflect__ App  {
public:

        App (std::string const &configurationFile) __tiliae_no_reflect__;
        ~App ();

        /*
         * Uruchamiamy funkcję start z kontrolerem.
         * show? navigate? Ta funkcja będzie używana w wielu miejscach aby przejść
         * na jakiś widok
         */
        void start (std::string const &unitName);

        /**
         * Merge another unit with unit that currently is active. It means that currently active unit
         * will not be discarded, but the two will be merged instead.
         */
        void join (std::string const &unitName);

        /**
         * Units to be substracted from current unit.
         */
        void split (std::string const &unitName);

        /**
         * Quits entire application regardless of controller nesting level. Programmer
         * (user of the library) invokes this to end the app.
         */
        void quit ();

        /*
         * There is always exactly one active unit which reacts to user requests (in fact its controllers do this). If we want to
         * navigate to other unit by invoking something like app->show ("otherController"), two
         * scenarios may happen:
         * - New controller replaces the old one. We can navigate back to the previous one by invoking
         * app->back ().
         * - New controller runs alongside with old one making a stack structure. Simply call app->end ()
         * in child controller to close it and its view and return to the root controller. Issuing
         * app->back () inside one of child controllers will result in returning to previous root controller.
         * back and forward methods operate only on root controllers.
         */
        void back ();

        /**
         * Users can request a submit using this action. It not perform any actions, only
         * sends an SubmitEvent to be processed in the next main loop iteration. doSubmit
         * performs all the work.
         */
        void submit (std::string const &viewName, std::string const &dataRange, std::string const &controllerName);

        /**
         * Handles quit request from GUI user (i.e. when he clicks widow manager's close button). Can be invoked
         * programmaticaly by the programmer as well, for example as a handler of "quit" menu action.
         */
        void userQuitRequest ();

        /**
         * Do the opposite of submit i.e. converts data from model suitable to be presented on the
         * view, and shows it there.
         * \param viewName Name of the view to populate with fresh data. If empty, all active views
         * will be populated.
         * \param dataRange Specifies which range of data to convert and display. Imagine you have
         * beefy table with tons of data. It would be pointless to convert all the contents if only
         * one tiny cell was changed. dataRange allows you to narrow the conversion process.
         */
        void refresh (std::string const &viewName, std::string const &dataRange);

        /**
         *
         */
        void show (std::string const &page) {}
        void hide (std::string const &page) {}

        Context &getContext ();
        static k202::K202 *getK202 ();
        static Wrapper::BeanWrapper *getBeanWrapper ();

        std::string getDefaultProperty (std::string const &widgetType) const;
        IUnit *getCurrentUnit ();
        Config const *getConfig () const;

private:

        /**
         * Gets a unit with given name. Hides implementation details, so units can be stored on some static
         * list or created with new. At this point this is not known.
         */
        IUnit *getUnit (std::string const &name);

        /**
         * Gets a page. Same notes as in case of Units apply.
         */
        Page *getPage (std::string const &name);

        /**
         * Deals with SubmitEvents.
         */
        void doSubmit (SubmitEvent *event);
        void doRefresh (RefreshEvent *event);

        /**
         * Creates a tiliae container instance (pointer is in Impl).
         */
        void createContainer (std::string const &configFile);

        /**
         * Method which has to be invoked in your main application loop.
         */
        void run ();

        ///
        Core::StringSet manageUnits ();
        void managePages (Core::StringSet const &viewCommands);

        void addPage (std::string const &page);
        void removePage (std::string const &page);
        void movePage (std::string const &pageA, std::string const &pageB);

        friend class SubmitEvent;
        friend class RefreshEvent;
        friend class QuitEvent;
        friend gboolean guiThread (gpointer user_data);

private:

        struct Impl;
        Impl *impl;
};

} // namespace GtkForms

#endif /* APP_H_ */
