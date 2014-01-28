/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef PAGE_H_
#define PAGE_H_

#include <ostream>
#include "ReflectionMacros.h"
#include "view/IView.h"
#include "IPage.h"

class Page : public IPage {
public:
        ctr__ (void)

        virtual ~Page () {}

//        PageOperationResult start (IPage *page);
        ViewMap join (IPage *page);
        ViewMap split (IPage *page);

        IView *getView (std::string const &viewName) { return views[viewName]; }

        ViewMap &getViews () { return views; }

        friend std::ostream &operator<< (std::ostream &o, Page const &u);

private:

        ViewMap prr_ (views);

        end_ (Page)
};

std::ostream &operator<< (std::ostream &o, Page const &p);

#endif /* PAGE_H_ */
