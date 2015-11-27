/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKFORMS_CONTEXT_H_
#define GTKFORMS_CONTEXT_H_

#include "controller/AbstractController.h"
#include "validator/IValidator.h"
#include <ReflectionParserAnnotation.h>

namespace GtkForms {

/**
 *
 */
class Context {
public:
        Context (Wrapper::BeanWrapper *w) : wrapper {w} {}
        virtual ~Context () {}

        Core::VariantMap &getSessionScope () { return sessionScope; }
        void clearSessionScope () { sessionScope.clear (); }

        void setToSessionScope (std::string const &path, Core::Variant v);
        void setToControllerScope (Core::VariantMap *controllerScope, std::string const &path, Core::Variant v);

        /**
         * @brief gets a data from curtrent controller (Context::setCurrentController).
         * @param name
         * @return
         */
//        Core::Variant get (const std::string &name);

        ValidationAndBindingResultContainer *getValidationResults () { return validationResults; }
        void setValidationResults (ValidationAndBindingResultContainer *r) { validationResults = r; }

//        void setCurrentController (AbstractController *c) { hierarchicalAccessor.setCurrentController (c); }
//        HierarchicalAccessor &getHierarchicalAccessor () { return hierarchicalAccessor; }

private:

        Core::VariantMap sessionScope;
        Wrapper::BeanWrapper *wrapper = nullptr;
        ValidationAndBindingResultContainer *validationResults = nullptr;
};

} // namespace GtkForms

#endif /* CONTEXT_H_ */
