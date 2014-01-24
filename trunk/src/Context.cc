/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Context.h"

Core::Variant Context::get (const std::string &name)
{
        Core::VariantMap::iterator i = flash.find (name);

        if (i != flash.end ()) {
                return i->second;
        }

        i = unit.find (name);

        if (i != unit.end ()) {
                return i->second;
        }

        i = session.find (name);

        if (i != session.end ()) {
                return i->second;
        }

        return Core::Variant ();
}

