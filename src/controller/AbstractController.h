/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ABSTRACTCONTROLLER_H_
#define ABSTRACTCONTROLLER_H_

#include "IController.h"
#include "ReflectionMacros.h"

namespace GtkForms {

class AbstractController : public IController {
public:
        REFLECTION_CLASS
        virtual ~AbstractController () {}

        App *getApp () { return app; }

        void setToSessionScope (std::string const &path, Core::Variant v);
        void setToUnitScope (std::string const &path, Core::Variant v);
        void setToFlashScope (std::string const &path, Core::Variant v);
        Core::VariantMap &getFlashScope () { return flash; }
        void clearFlashScope () { flash.clear (); }
        Core::Variant get (const std::string &name);

        virtual void onQuit () {}
        virtual void onIdle () {}
        virtual ValidationAndBindingResult validate () { return ValidationAndBindingResult {}; }

        SignalAdapterVector &getSignalAdapters () { return signalAdapters; }
        ValidatorVector const &getValidators () const { return validators; }

        int getLoopDelayMs () const { return loopDelayMs; }
        int &getLastMs () { return lastMs; }

        ValidationAndBindingResultContainer const &getValidationResults () const { return validationResults; }
        ValidationAndBindingResultContainer &getValidationResults () { return validationResults; }

protected:

        AbstractController () : loopDelayMs {-1} {}
        App *app = 0;

private:

        void setApp (App *app) { this->app = app; }

private:

        ValidatorVector REFLECTION_FIELD_REFERENCE_INPLACE (validators);
        SignalAdapterVector REFLECTION_FIELD_REFERENCE_INPLACE (signalAdapters);
        int REFLECTION_FIELD_VALUE_INPLACE (loopDelayMs);
        int lastMs = 0;
        Core::VariantMap flash;
        ValidationAndBindingResultContainer validationResults;

        REFLECTION_END (AbstractController)
};

} // namespace GtkForms

#endif /* ABSTRACTCONTROLLER_H_ */
