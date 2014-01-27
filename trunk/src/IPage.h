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

/**
 * See documentation of IUnit. Same reasoning apply here.
 */
class IPage : public Core::Object {
public:
        virtual ~IPage () {}

        virtual ViewMap replace (IPage *unit) = 0;
        virtual ViewMap remove (IPage *unit) = 0;
        virtual IView *getView (std::string const &viewName) = 0;
        virtual ViewMap &getViews () = 0;
};

#endif /* IPAGE_H_ */
