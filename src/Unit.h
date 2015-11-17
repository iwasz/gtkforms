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
#include <ReflectionParserAnnotation.h>
#include "signalAdapter/AbstractSignalAdapter.h"

namespace GtkForms {

/**
 * Single unit of work which consists of one or more controllers. Only one unit can be
 * active at a time, but two or more units can be merged to form a bigger unit (it retains
 * all the controllers of all units in such case).
 */
class __tiliae_reflect__ Unit : public IUnit {
public:
        virtual ~Unit () {}

        UnitOperationResult start (IUnit *unit);
        UnitOperationResult join (IUnit *unit);
        UnitOperationResult split (IUnit *unit);

        IController *getController (std::string const &controllerName);

        ControllerMap &getControllers () __tiliae_no_reflect__ { return controllers; }
        ControllerMap const &getControllers () const __tiliae_no_reflect__ { return controllers; }

        SignalAdapterVector getSignalAdapters () __tiliae_no_reflect__;

// private:

        ControllerMap controllers;
};

std::ostream &operator<< (std::ostream &o, IUnit const &u);

} // namespace GtkForms

#endif /* UNIT_H_ */
