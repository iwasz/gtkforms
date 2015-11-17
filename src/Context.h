/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKFORMS_CONTEXT_H_
#define GTKFORMS_CONTEXT_H_

#include "controller/IController.h"
#include "validator/IValidator.h"
#include <ReflectionParserAnnotation.h>
#include "Unit.h"

namespace GtkForms {

class __tiliae_reflect__ AbstractAccessor {
public:

        AbstractAccessor (Core::VariantMap *s, Core::VariantMap *u) : sessionScope {s}, unitScope {u} {}
        virtual ~AbstractAccessor () {}
        virtual Core::Variant get (const std::string &name) = 0;
        virtual void set (const std::string &name, Core::Variant v) = 0;

        void setSessionScope (Core::VariantMap *s) { sessionScope = s; }
        void setUnitScope (Core::VariantMap *u) { unitScope = u; }
        void setCurrentController (IController *c) { currentController = c; }

protected:

        Core::VariantMap *sessionScope = nullptr;
        Core::VariantMap *unitScope = nullptr;
        IController *currentController = nullptr;

};

class __tiliae_reflect__ AllFlashAccessor : public AbstractAccessor {
public:

        AllFlashAccessor (Core::VariantMap *s, Core::VariantMap *u) __tiliae_no_reflect__ : AbstractAccessor (s, u) {}
        virtual ~AllFlashAccessor () {}

        Core::Variant get (const std::string &name);
        void set (const std::string &name, Core::Variant v);

        void setCurrentUnit (Unit *u) { currentUnit = u; }

private:

        Unit *currentUnit = nullptr;
};

class __tiliae_reflect__ SingleFlashAccessor : public AbstractAccessor {
public:
        SingleFlashAccessor (Core::VariantMap *s, Core::VariantMap *u) __tiliae_no_reflect__ : AbstractAccessor (s, u) {}
        virtual ~SingleFlashAccessor () {}

        Core::Variant get (const std::string &name);
        void set (const std::string &name, Core::Variant v);
};

/**
 *
 */
class Context {
public:
        Context (Wrapper::BeanWrapper *w) : wrapper {w} {}
        virtual ~Context () {}

        Core::VariantMap &getSessionScope () { return sessionScope; }
        Core::VariantMap &getUnitScope () { return unitScope; }

        void clearSessionScope () { sessionScope.clear (); }
        void clearUnitScope () { unitScope.clear (); }

        void setToSessionScope (std::string const &path, Core::Variant v);
        void setToUnitScope (std::string const &path, Core::Variant v);
        void setToFlashScope (Core::VariantMap *flash, std::string const &path, Core::Variant v);

        Core::Variant get (const std::string &name);

        ValidationAndBindingResultContainer *getValidationResults () { return validationResults; }
        void setValidationResults (ValidationAndBindingResultContainer *r) { validationResults = r; }

        void setCurrentUnit (Unit *u) { allFlashAccessor.setCurrentUnit (u); }
        void setCurrentController (IController *c) { singleFlashAccessor.setCurrentController (c); allFlashAccessor.setCurrentController (c); }

        AllFlashAccessor &getAllFlashAccessor () { return allFlashAccessor; }
        SingleFlashAccessor &getSingleFlashAccessor () { return singleFlashAccessor; }

private:

        Core::VariantMap sessionScope;
        Core::VariantMap unitScope;
        Wrapper::BeanWrapper *wrapper;
        AllFlashAccessor allFlashAccessor {&sessionScope, &unitScope};
        SingleFlashAccessor singleFlashAccessor {&sessionScope, &unitScope};
        ValidationAndBindingResultContainer *validationResults = nullptr;
};

} // namespace GtkForms

#endif /* CONTEXT_H_ */
