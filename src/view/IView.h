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

class IView {
public:
        virtual ~IView () {}
        virtual std::string getName () const = 0;
        virtual void show () = 0;
        virtual void hide () = 0;
};

typedef std::map <std::string, IView *> ViewMap;

#endif /* IVIEW_H_ */
