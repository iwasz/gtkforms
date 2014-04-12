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
        Core::VariantMap::iterator i;

        if (fromAllFlashes) {
                ControllerMap &cMap = currentUnit->getControllers ();

                for (ControllerMap::value_type elem : cMap) {
                        IController *ctr = elem.second;
                        i = ctr->getFlashScope ().find (name);

                        if (i != ctr->getFlashScope ().end ()) {
                                return i->second;
                        }
                }
        }
        else {
                i = currentController->getFlashScope ().find (name);

                if (i != currentController->getFlashScope ().end ()) {
                        return i->second;
                }
        }

        i = unitScope.find (name);

        if (i != unitScope.end ()) {
                return i->second;
        }

        i = sessionScope.find (name);

        if (i != sessionScope.end ()) {
                return i->second;
        }

        return Core::Variant ();
}

void ContextPriv::set (const std::string &name, Core::Variant v)
{
        currentController->getFlashScope ()[name] = v;
}

/*##########################################################################*/

Core::Variant Context::get (const std::string &name)
{
        contextPriv.setFromAllFlashes (false);
        wrapper->setWrappedObject (Core::Variant (&contextPriv));
        Core::DebugContext ctx;
        bool error = false;
        Core::Variant v = wrapper->get (name, &error, &ctx);

        if (error) {
                Core::Exception e ("Context::get failed. name = [" + name + "]");
                e.addContext (ctx);
                contextPriv.setFromAllFlashes (true);
                throw e;
        }

        contextPriv.setFromAllFlashes (true);
        return v;
}

void Context::setToSessionScope (std::string const &path, Core::Variant v)
{
        wrapper->setWrappedObject (Core::Variant {&contextPriv.getSessionScope ()});
        Core::DebugContext ctx;

        if (!wrapper->set (path, v)) {
                Core::Exception e ("Context::setToSessionScope failed. path = [" + path + "]");
                e.addContext (ctx);
                throw e;
        }
}

void Context::setToUnitScope (std::string const &path, Core::Variant v)
{
        wrapper->setWrappedObject (Core::Variant {&contextPriv.getUnitScope ()});
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
