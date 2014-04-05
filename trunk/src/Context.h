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

namespace GtkForms {

class ContextPriv {
public:
        abt__

        virtual ~ContextPriv () {}

        Core::VariantMap &getSessionScope () { return session; }
        Core::VariantMap &getUnitScope () { return unit; }
        Core::VariantMap &getFlashScope () { return flash; }

        mth_ (get) Core::Variant get (const std::string &name);

private:

        Core::VariantMap session;
        Core::VariantMap unit;
        Core::VariantMap flash;



        end_ (ContextPriv)
};

/**
 *
 */
class Context {
public:
        abt__
        Context (Wrapper::BeanWrapper *w) : wrapper {w} {}
        virtual ~Context () {}

        Core::VariantMap &getSessionScope () { return contextPriv.getSessionScope (); }
        Core::VariantMap &getUnitScope () { return contextPriv.getUnitScope (); }
        Core::VariantMap &getFlashScope () { return contextPriv.getFlashScope (); }

        void clearSessionScope () { contextPriv.getSessionScope ().clear (); }
        void clearUnitScope () { contextPriv.getUnitScope ().clear (); }
        void clearFlashScope () { contextPriv.getFlashScope ().clear (); }

        void setToSessionScope (std::string const &path, Core::Variant v);
        void setToUnitScope (std::string const &path, Core::Variant v);
        void setToFlashScope (std::string const &path, Core::Variant v);

        mth_ (get) Core::Variant get (const std::string &name);

        ValidationResultVector const &getValidationResults () const { return validationResults; }
        ValidationResultVector &getValidationResults () { return validationResults; }
        void clearValidationResults () { validationResults.clear (); }

private:

        Wrapper::BeanWrapper *wrapper = nullptr;
        ContextPriv contextPriv;
        ValidationResultVector validationResults;


        end_ (Context)
};

} // namespace GtkForms

#endif /* CONTEXT_H_ */
