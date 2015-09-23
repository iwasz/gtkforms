/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef IVIEW_H_
#define IVIEW_H_

#include <string>
#include <map>
#include <gtk/gtk.h>
#include "ReflectionMacros.h"

namespace GtkForms {
class Context;
class App;

class IView : public Core::Object {
public:
        virtual ~IView () {}
        virtual std::string getName () const = 0;

        virtual void loadUi (App *app) = 0;

        /**
         * Shows for the first time, or if widget is hidden.
         */
        virtual void show () = 0;

        /**
         * Temporarily hides the widget. No memory is freed.
         */
        virtual void hide () = 0;

        /**
         * Hides and unloads this widget from memory.
         */
        virtual void destroyUi () = 0;

        /**
         * Called on RefreshEvent.
         */
        virtual void refresh (Context *ctx) = 0;

        typedef std::map <std::string, GtkWidget *> InputMap;
        virtual InputMap getInputs (std::string const &dataRange, bool outputs = false) = 0;

        virtual bool isLoaded () const = 0;
};

typedef std::map <std::string, IView *> ViewMap;
REFLECTION_COLLECTION (ViewMap)

typedef std::set <IView *> ViewSet;

} // namespace GtkForms

#endif /* IVIEW_H_ */
