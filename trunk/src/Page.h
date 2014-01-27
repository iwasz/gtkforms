/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef PAGE_H_
#define PAGE_H_

#include "ReflectionMacros.h"
#include "view/IView.h"
#include "IPage.h"

class Page : public IPage {
public:
        ctr__ (void)

        virtual ~Page () {}

        ViewMap replace (IPage *unit);

        ViewMap remove (IPage *unit);

        IView *getView (std::string const &viewName) { return views[viewName]; }

        ViewMap &getViews () { return views; }

private:

        ViewMap views;

        end_ (Page)
};

#endif /* PAGE_H_ */
