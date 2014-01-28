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

namespace GtkForms {

class Page : public IPage {
public:
        ctr__ (void)

        Page () : joinable {false} {}
        virtual ~Page () {}

        PageOperationResult start (IPage *page);
        PageOperationResult join  (IPage *page);
        PageOperationResult split (IPage *page);

        IView *getView (std::string const &viewName) { return views[viewName]; }
        ViewMap &getViews () { return views; }

        bool getJoinable () const { return joinable; }
        void setJoinable (bool b) { joinable = b; }

        friend std::ostream &operator<< (std::ostream &o, Page const &u);

private:

        ViewMap prr_ (views);
        bool prr_ (joinable);

        end_ (Page)
};

std::ostream &operator<< (std::ostream &o, Page const &p);

} // namespace GtkForms

#endif /* PAGE_H_ */
