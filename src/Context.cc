/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Context.h"

namespace GtkForms {

Core::Variant ContextPriv::get (const std::string &name)
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

Core::Variant Context::get (const std::string &name)
{
        wrapper->setWrappedObject (Core::Variant (&contextPriv));
        return wrapper->get (name);
}

void Context::setToSessionScope (std::string const &path, Core::Variant v)
{
        wrapper->setWrappedObject (Core::Variant {&contextPriv.getSessionScope ()});
        wrapper->set (path, v);
}

void Context::setToUnitScope (std::string const &path, Core::Variant v)
{
        wrapper->setWrappedObject (Core::Variant {&contextPriv.getUnitScope ()});
        wrapper->set (path, v);
}

void Context::setToFlashScope (std::string const &path, Core::Variant v)
{
        wrapper->setWrappedObject (Core::Variant {&contextPriv.getFlashScope ()});
        wrapper->set (path, v);
}

} // namespace GtkForms
