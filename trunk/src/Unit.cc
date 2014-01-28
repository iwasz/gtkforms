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
                        i = controllers.erase (i);
                        result.removed[i->first] = i->second;
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

ostream &operator<< (ostream &o, Unit const &u)
{
        o << "Unit [";

        for (auto i = u.controllers.begin (); i != u.controllers.end (); ) {
                o << i->second->getName ();

                if (++i != u.controllers.end ()) {
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
