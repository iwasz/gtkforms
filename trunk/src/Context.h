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
#include "controller/IController.h"
#include "validator/IValidator.h"
#include "ReflectionMacros.h"
#include "Unit.h"

namespace GtkForms {

class ContextPriv {
public:
        abt__
        virtual ~ContextPriv () {}

        Core::VariantMap &getSessionScope () { return sessionScope; }
        Core::VariantMap &getUnitScope () { return unitScope; }

        mth_ (get) Core::Variant get (const std::string &name);
        void set (const std::string &name, Core::Variant v);

        void setCurrentUnit (Unit *u) { currentUnit = u; }
        void setCurrentController (IController *c) { currentController = c; }
        void setFromAllFlashesn (bool b) { fromAllFlashes = b; }

private:

        Core::VariantMap sessionScope;
        Core::VariantMap unitScope;

        Unit *currentUnit = nullptr;
        IController *currentController = nullptr;
        bool fromAllFlashes = true;

        end_ (ContextPriv)
};

/**
 *
 */
class Context {
public:
        Context (Wrapper::BeanWrapper *w) : wrapper {w} {}
        virtual ~Context () {}

        Core::VariantMap &getSessionScope () { return contextPriv.getSessionScope (); }
        Core::VariantMap &getUnitScope () { return contextPriv.getUnitScope (); }

        void clearSessionScope () { contextPriv.getSessionScope ().clear (); }
        void clearUnitScope () { contextPriv.getUnitScope ().clear (); }

        void setToSessionScope (std::string const &path, Core::Variant v);
        void setToUnitScope (std::string const &path, Core::Variant v);
        void setToFlashScope (Core::VariantMap *flash, std::string const &path, Core::Variant v);

        Core::Variant get (const std::string &name);

        ValidationResultVector const &getValidationResults () const { return validationResults; }
        ValidationResultVector &getValidationResults () { return validationResults; }
        void clearValidationResults () { validationResults.clear (); }

        void setCurrentUnit (Unit *u) { contextPriv.setCurrentUnit (u); }
        void setCurrentController (IController *c) { contextPriv.setCurrentController (c); }
        void setFromAllFlashesn (bool b) { contextPriv.setFromAllFlashesn (b); }
        ContextPriv &getContextPriv () { return contextPriv; }

private:

        Wrapper::BeanWrapper *wrapper = nullptr;
        ContextPriv contextPriv;
        ValidationResultVector validationResults;
};

} // namespace GtkForms

#endif /* CONTEXT_H_ */
