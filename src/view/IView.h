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

class IView : public Core::Object {
public:
        virtual ~IView () {}
        virtual std::string getName () const = 0;
        virtual void show () = 0;
        virtual void hide () = 0;

        virtual void model2View (std::string const &dataRange) = 0;
        virtual void view2Model (std::string const &dataRange) = 0;

};

typedef std::map <std::string, IView *> ViewMap;
col_ (ViewMap)

#endif /* IVIEW_H_ */
