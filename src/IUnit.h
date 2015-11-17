/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef IUNIT_H_
#define IUNIT_H_

#include "controller/IController.h"
#include "signalAdapter/AbstractSignalAdapter.h"

namespace GtkForms {

/**
 * Stores information which controllers were added and which were removed durnig App operatons
 * (statr, join and split).
 */
struct UnitOperationResult {
        ControllerMap added;
        ControllerMap removed;

        UnitOperationResult &operator += (UnitOperationResult const &);
};

/**
 * Why interface? Beczause one could want to reimplement some functionality of
 * units. Imagine for example units that are conditionally enabled and could
 * provide debig stuff or sth.
 */
class __tiliae_reflect__ IUnit : public Core::Object {
public:
        virtual ~IUnit () {}

        /**
         * Start a unit.
         */
        virtual UnitOperationResult start (IUnit *unit) = 0;

        /**
         * Add another unit to this unit. After this operstion it will have all its
         * original controllers and the newly added unit's controllers.
         * \return List of newly added controllers. If unit you are about to add contains
         * a controller which is already present in this unit, it will not be added, and
         * won't be returned in this list. Only controllers that are new will be returned;
         */
        virtual UnitOperationResult join (IUnit *unit) = 0;
        virtual UnitOperationResult split (IUnit *unit) = 0;
        virtual IController *getController (std::string const &controllerName) __tiliae_no_reflect__ = 0;
        virtual ControllerMap &getControllers () __tiliae_no_reflect__ = 0;
        virtual ControllerMap const &getControllers () const __tiliae_no_reflect__ = 0;
        virtual SignalAdapterVector getSignalAdapters () __tiliae_no_reflect__ = 0;
};

} // namespace GtkForms

#endif /* IUNIT_H_ */
