#include "HierarchicalAccessor.h"

namespace GtkForms {

Core::Variant HierarchicalAccessor::get (const std::string &name)
{
        AbstractController *controller = currentController;

        if (name == "controller") {
                return Core::Variant (controller);
        }
        else if (name == "parent" || name == "controller.parent") {
                return Core::Variant (controller->getParent ());
        }

        Core::VariantMap::iterator i;

        if (!currentController) {
                throw new Core::Exception ("HierarchicalAccessor::get : currentController is NULL.");
        }

        do {
                i = currentController->getControllerScope ().find (name);

                if (i != currentController->getControllerScope ().end ()) {
                        return i->second;
                }
        } while ((controller = controller->getParent ()));

        i = sessionScope->find (name);

        if (i != sessionScope->end ()) {
                return i->second;
        }

        return Core::Variant ();
}

/*****************************************************************************/

void HierarchicalAccessor::set (const std::string &name, Core::Variant v)
{
        if (!currentController) {
                throw new Core::Exception ("HierarchicalAccessor::get : currentController is NULL.");
        }

        currentController->getControllerScope ()[name] = v;
}
}
