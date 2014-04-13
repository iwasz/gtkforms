/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Context.h"

namespace GtkForms {

Core::Variant AllFlashAccessor::get (const std::string &name)
{
        Core::VariantMap::iterator i;
        ControllerMap &cMap = currentUnit->getControllers ();

        for (ControllerMap::value_type elem : cMap) {
                IController *ctr = elem.second;
                i = ctr->getFlashScope ().find (name);

                if (i != ctr->getFlashScope ().end ()) {
                        return i->second;
                }
        }

        i = unitScope->find (name);

        if (i != unitScope->end ()) {
                return i->second;
        }

        i = sessionScope->find (name);

        if (i != sessionScope->end ()) {
                return i->second;
        }

        return Core::Variant ();
}

void AllFlashAccessor::set (const std::string &name, Core::Variant v)
{
        currentController->getFlashScope ()[name] = v;
}

/*##########################################################################*/

Core::Variant SingleFlashAccessor::get (const std::string &name)
{
        Core::VariantMap::iterator i;
        i = currentController->getFlashScope ().find (name);

        if (i != currentController->getFlashScope ().end ()) {
                return i->second;
        }

        i = unitScope->find (name);

        if (i != unitScope->end ()) {
                return i->second;
        }

        i = sessionScope->find (name);

        if (i != sessionScope->end ()) {
                return i->second;
        }

        return Core::Variant ();
}

void SingleFlashAccessor::set (const std::string &name, Core::Variant v)
{
        currentController->getFlashScope ()[name] = v;
}

/*##########################################################################*/

Core::Variant Context::get (const std::string &name)
{
        wrapper->setWrappedObject (Core::Variant (&singleFlashAccessor));
        Core::DebugContext ctx;
        bool error = false;
        Core::Variant v = wrapper->get (name, &error, &ctx);

        if (error) {
                Core::Exception e ("Context::get failed. name = [" + name + "]");
                e.addContext (ctx);
                throw e;
        }

        return v;
}

void Context::setToSessionScope (std::string const &path, Core::Variant v)
{
        wrapper->setWrappedObject (Core::Variant {&getSessionScope ()});
        Core::DebugContext ctx;

        if (!wrapper->set (path, v)) {
                Core::Exception e ("Context::setToSessionScope failed. path = [" + path + "]");
                e.addContext (ctx);
                throw e;
        }
}

void Context::setToUnitScope (std::string const &path, Core::Variant v)
{
        wrapper->setWrappedObject (Core::Variant {&getUnitScope ()});
        Core::DebugContext ctx;

        if (!wrapper->set (path, v)) {
                Core::Exception e ("Context::setToSessionScope failed. path = [" + path + "]");
                e.addContext (ctx);
                throw e;
        }
}

void Context::setToFlashScope (Core::VariantMap *flash, std::string const &path, Core::Variant v)
{
        wrapper->setWrappedObject (Core::Variant {flash});
        Core::DebugContext ctx;

        if (!wrapper->set (path, v)) {
                Core::Exception e ("Context::setToSessionScope failed. path = [" + path + "]");
                e.addContext (ctx);
                throw e;
        }
}

} // namespace GtkForms
