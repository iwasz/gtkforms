/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_PAGE_H_
#define GTK_FORMS_PAGE_H_

#include <ostream>
#include "ReflectionMacros.h"
#include "view/IView.h"
#include "IPage.h"
#include "view/GtkTile.h"
#include "view/GtkView.h"
#include "view/Slot.h"

namespace GtkForms {

class Page : public IPage {
public:
        ctr__ (void)

//        Page () : joinable {false} {}
        virtual ~Page () {}

//        PageOperationResult start (IPage *page);
//        PageOperationResult join  (IPage *page);
//        PageOperationResult split (IPage *page);

//        IView *getView (std::string const &viewName) { return views[viewName]; }
//        ViewMap &getViews () { return views; }

        GtkTileMap getTiles () { return tiles; }
        GtkView *getView () { return view; }
        std::string getName () const { return name; }
        SlotVector getSlots () { return slots; }

        void loadUi (Context *context);
        void destroyUi ();

//        bool getJoinable () const { return joinable; }
//        void setJoinable (bool b) { joinable = b; }

        friend std::ostream &operator<< (std::ostream &o, Page const &u);

private:

        GtkTileMap prr_ (tiles);
        GtkView *prp_ (view);
        std::string prr_ (name);
        SlotVector prr_ (slots);

//        ViewMap prr_ (views);
//        bool prr_ (joinable);

        end_ (Page)
};

//std::ostream &operator<< (std::ostream &o, Page const &p);

} // namespace GtkForms

#endif /* PAGE_H_ */
