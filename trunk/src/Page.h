/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef PAGE_H_
#define PAGE_H_

#include "view/IView.h"

class Page {
public:
        Page ();
        virtual ~Page ();

        std::string getName () const {}

        ViewMap replace (Page *unit) {}

        ViewMap remove (Page *unit) {}

        IView *getView (std::string const &viewName) { return views[viewName]; }

private:

        ViewMap views;
};

#endif /* PAGE_H_ */