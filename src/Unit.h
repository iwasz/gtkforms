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
#include "IUnit.h"
#include "ReflectionMacros.h"

namespace GtkForms {

/**
 * Single unit of work which consists of one or more controllers. Only one unit can be
 * active at a time, but two or more units can be merged to form a bigger unit (it retains
 * all the controllers of all units in such case).
 */
class Unit : public IUnit {
public:
        ctr__ (void)

        virtual ~Unit () {}

        UnitOperationResult start (IUnit *unit);
        UnitOperationResult join (IUnit *unit);
        UnitOperationResult split (IUnit *unit);

        IController *getController (std::string const &controllerName) { return controllers[controllerName]; }

        ControllerMap &getControllers () { return controllers; }
        ControllerMap const &getControllers () const { return controllers; }

private:

        ControllerMap prr_ (controllers);

        end_ (Unit)
};

std::ostream &operator<< (std::ostream &o, IUnit const &u);

} // namespace GtkForms

#endif /* UNIT_H_ */
