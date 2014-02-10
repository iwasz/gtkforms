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
#include "ReflectionMacros.h"

namespace GtkForms {
class Context;

class IView : public Core::Object {
public:
        virtual ~IView () {}
        virtual std::string getName () const = 0;

        virtual void loadUi (Context *context) = 0;

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
        virtual void destroy () = 0;

//        virtual void model2View (std::string const &dataRange) = 0;
//        virtual void view2Model (std::string const &dataRange) = 0;

};

typedef std::map <std::string, IView *> ViewMap;
col_ (ViewMap)

} // namespace GtkForms

#endif /* IVIEW_H_ */
