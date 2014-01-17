/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef UNIT_H_
#define UNIT_H_

#include "controller/IController.h"

/**
 * Stores information which controllers were added and which were removed durnig Unit operatons
 * (add, remove and morph).
 */
struct UnitOperationResult {
        ControllerMap added;
        ControllerMap removed;

        UnitOperationResult &operator += (UnitOperationResult const &) { return *this; }
};

/**
 * Single unit of work which consists of one or more controllers. Only one unit can be
 * active at a time, but two or more units can be merged to form a bigger unit (it retains
 * all the controllers of all units in such case).
 */
class Unit {
public:
        Unit ();
        virtual ~Unit ();

        /**
         * Add another unit to this unit. After this operstion it will have all its
         * original controllers and the newly added unit's controllers.
         * \return List of newly added controllers. If unit you are about to add contains
         * a controller which is already present in this unit, it will not be added, and
         * won't be returned in this list. Only controllers that are new will be returned;
         */
        UnitOperationResult add (Unit *unit) {}

        UnitOperationResult replace (Unit *unit) {}

        UnitOperationResult remove (Unit *unit) {}

        std::string getName () const {}

        IController *getController (std::string const &controllerName) { return controllers[controllerName]; }

private:

//        ControllerVector controllers;
        ControllerMap controllers;
};

#endif /* UNIT_H_ */
