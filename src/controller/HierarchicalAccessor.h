#ifndef HIERARCHICALACCESSOR_H
#define HIERARCHICALACCESSOR_H

#include "AbstractAccessor.h"

namespace GtkForms {

/**
 * @brief The HierarchicalAccessor class traverses controller hierarchy from bottom (starting from the
 * controller which invoked the call) to top (app->rootController). If object searched for is not found,
 * then Context::sessionScope is searched. If all above fails, an empty Core::Variant is returned.
 */
class __tiliae_reflect__ HierarchicalAccessor : public AbstractAccessor {
public:

        HierarchicalAccessor (Core::VariantMap *s = nullptr) __tiliae_no_reflect__ : AbstractAccessor (s) {}
        virtual ~HierarchicalAccessor () {}

        Core::Variant get (const std::string &name);
        void set (const std::string &name, Core::Variant v);
};

}

#endif // HIERARCHICALACCESSOR_H
