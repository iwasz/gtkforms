/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKFORMS_CONTEXT_H_
#define GTKFORMS_CONTEXT_H_

#include <Tiliae.h>
#include "validator/IValidator.h"
#include "ReflectionMacros.h"
#include "Unit.h"

namespace GtkForms {

class ContextPriv {
public:
        abt__
        ContextPriv (Unit **u) : unit {u} {}
        virtual ~ContextPriv () {}

        Core::VariantMap &getSessionScope () { return sessionScope; }
        Core::VariantMap &getUnitScope () { return unitScope; }

        mth_ (get) Core::Variant get (const std::string &name);

private:

        Core::VariantMap sessionScope;
        Core::VariantMap unitScope;
        Unit **unit = nullptr;
        end_ (ContextPriv)
};

/**
 *
 */
class Context {
public:
        abt__
        Context (Wrapper::BeanWrapper *w, Unit *u) : wrapper {w}, unit {u} {}
        virtual ~Context () {}

        Core::VariantMap &getSessionScope () { return contextPriv.getSessionScope (); }
        Core::VariantMap &getUnitScope () { return contextPriv.getUnitScope (); }

        void clearSessionScope () { contextPriv.getSessionScope ().clear (); }
        void clearUnitScope () { contextPriv.getUnitScope ().clear (); }

        void setToSessionScope (std::string const &path, Core::Variant v);
        void setToUnitScope (std::string const &path, Core::Variant v);
        void setToFlashScope (Core::VariantMap *flash, std::string const &path, Core::Variant v);

        mth_ (get) Core::Variant get (const std::string &name);

        ValidationResultVector const &getValidationResults () const { return validationResults; }
        ValidationResultVector &getValidationResults () { return validationResults; }
        void clearValidationResults () { validationResults.clear (); }

private:

        Wrapper::BeanWrapper *wrapper = nullptr;
        Unit *unit = nullptr;
        ContextPriv contextPriv {&unit};
        ValidationResultVector validationResults;
        end_ (Context)
};

} // namespace GtkForms

#endif /* CONTEXT_H_ */
