/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Context.h"

namespace GtkForms {

void Context::setToControllerScope (Core::VariantMap *controllerScope, std::string const &path, Core::Variant v)
{
        wrapper->setWrappedObject (Core::Variant{ controllerScope });
        Core::DebugContext ctx;

        if (!wrapper->set (path, v)) {
                Core::Exception e ("Context::setToSessionScope failed. path = [" + path + "]");
                e.addContext (ctx);
                throw e;
        }
}

/*****************************************************************************/

void Context::setToSessionScope (std::string const &path, Core::Variant v)
{
        wrapper->setWrappedObject (Core::Variant{ &getSessionScope () });
        Core::DebugContext ctx;

        if (!wrapper->set (path, v)) {
                Core::Exception e ("Context::setToSessionScope failed. path = [" + path + "]");
                e.addContext (ctx);
                throw e;
        }
}

} // namespace GtkForms
