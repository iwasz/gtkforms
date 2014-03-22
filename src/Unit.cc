/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <algorithm>
#include <sstream>
#include "Unit.h"

namespace GtkForms {
using namespace std;
//using namespace std::placeholders;

/*--------------------------------------------------------------------------*/

UnitOperationResult Unit::join (IUnit *unit)
{
        ControllerMap &ctmap = unit->getControllers ();
        UnitOperationResult result;

        for (ControllerMap::value_type &entry : ctmap) {
                if (controllers.find (entry.first) == controllers.end ()) {
                        controllers[entry.first] = entry.second;
                        result.added[entry.first] = entry.second;
                }
        }

        return result;
}

/*--------------------------------------------------------------------------*/

UnitOperationResult Unit::start (IUnit *unit)
{
        ControllerMap &ctmap = unit->getControllers ();
        UnitOperationResult result;

        for (auto i = controllers.begin (); i != controllers.end (); ) {
                if (ctmap.find (i->first) == ctmap.end ()) {
                        result.removed[i->first] = i->second;
                        i = controllers.erase (i);
                }
                else {
                        ++i;
                }
        }

        for (ControllerMap::value_type &entry : ctmap) {
                if (controllers.find (entry.first) == controllers.end ()) {
                        controllers[entry.first] = entry.second;
                        result.added[entry.first] = entry.second;
                }
        }

        return result;
}

/*--------------------------------------------------------------------------*/

UnitOperationResult Unit::split (IUnit *unit)
{
        ControllerMap &ctmap = unit->getControllers ();
        UnitOperationResult result;

        for (ControllerMap::value_type &entry : ctmap) {
                if (controllers.find (entry.first) != controllers.end ()) {
                        controllers.erase (entry.first);
                        result.removed[entry.first] = entry.second;
                }
        }

        return result;
}

/*--------------------------------------------------------------------------*/

ostream &operator<< (ostream &o, IUnit const &u)
{
        o << "Unit [";

        for (auto i = u.getControllers ().begin (); i != u.getControllers ().end (); ) {
                o << i->first;

                if (++i != u.getControllers ().end ()) {
                        o << ", ";
                }
        }

        o << "]";
        return o;
}

/*--------------------------------------------------------------------------*/

UnitOperationResult &UnitOperationResult::operator += (UnitOperationResult const &uor)
{
        copy (uor.added.begin (), uor.added.end (), inserter (added, added.end ()));
        copy (uor.removed.begin (), uor.removed.end (), inserter (removed, removed.end ()));
        return *this;
}

/*--------------------------------------------------------------------------*/

IController *Unit::getController (std::string const &controllerName)
{
        ControllerMap::iterator i;
        if ((i = controllers.find (controllerName)) == controllers.end ()) {
                return 0;
        }

        return i->second;
}

/*--------------------------------------------------------------------------*/

SignalAdapterVector Unit::getSignalAdapters ()
{
        SignalAdapterVector adapters;

        for (ControllerMap::value_type &entry : getControllers ()) {
                std::copy (entry.second->getSignalAdapters ().begin (), entry.second->getSignalAdapters ().end (), std::back_inserter (adapters));
        }

        return adapters;
}

} // namespace GtkForms
