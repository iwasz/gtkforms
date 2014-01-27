/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Page.h"

ViewMap Page::replace (IPage *page)
{
        ViewMap &ctmap = page->getViews ();
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

ViewMap Page::remove (IPage *unit)
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
