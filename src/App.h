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

/**
 * Main object (singleton) of a GtkForms application. It can perofrm operations on main aspects
 * of the app, most notably it manages Units and Pages.
 */
class App {
public:

        App (std::string const &configurationFile);
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

//        /**
//         * Ends current unit. If this is a root controller, whole application
//         * gets closed.
//         */
//        void end ()
//        {
//
//        }

        /**
         * Quits entire application regardless of controller nesting level.
         */
        void quit ()
        {

        }

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
        void back ()
        {

        }

        void forward ()
        {

        }

        /**
         *
         */
        void show (std::string const &page) {}
        void hide (std::string const &page) {}

        /**
         * Method which has to be invoked in your main application loop.
         */
        void run ();

private:

        struct Impl;
        Impl *impl;

//        IController *currentController
//        IView *currentView;
//        Session session;
//        Flash flash;
};

#endif /* APP_H_ */
