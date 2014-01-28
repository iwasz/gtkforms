/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef IPAGE_H_
#define IPAGE_H_

#include "core/Object.h"

namespace GtkForms {

///**
// * Stores information which views were added and which were removed durnig App operatons
// * (start, join and split).
// */
//struct PageOperationResult {
//        ViewMap added;
//        ViewMap removed;
//
//        PageOperationResult &operator += (PageOperationResult const &);
//};

/**
 * See documentation of IUnit. Same reasoning apply here.
 */
class IPage : public Core::Object {
public:
        virtual ~IPage () {}

//        virtual PageOperationResult start (IPage *unit) = 0;

        /**
         * Returns views to show.
         */
        virtual ViewMap join (IPage *unit) = 0;

        /**
         * Returns views to hide.
         */
        virtual ViewMap split (IPage *unit) = 0;
        virtual IView *getView (std::string const &viewName) = 0;
        virtual ViewMap &getViews () = 0;
};

} // namespace GtkForms

#endif /* IPAGE_H_ */
