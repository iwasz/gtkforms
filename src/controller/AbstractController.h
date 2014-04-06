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
        abt__
        virtual ~AbstractController () {}

        App *getApp () { return app; }

        void setToSessionScope (std::string const &path, Core::Variant v);
        void setToUnitScope (std::string const &path, Core::Variant v);
        void setToFlashScope (std::string const &path, Core::Variant v);
        Core::Variant get (const std::string &name);

        virtual void onQuit () {}
        virtual void onIdle () {}
        virtual ValidationAndBindingResult validate () { return ValidationAndBindingResult {}; }

        SignalAdapterVector &getSignalAdapters () { return signalAdapters; }
        ValidatorVector const &getValidators () const { return validators; }

        int getLoopDelayMs () const { return loopDelayMs; }
        int &getLastMs () { return lastMs; }

protected:

        AbstractController () : loopDelayMs {-1} {}
        App *app = 0;

private:

        void setApp (App *app) { this->app = app; }

private:

        ValidatorVector prr_ (validators);
        SignalAdapterVector prr_ (signalAdapters);
        int prp_ (loopDelayMs);
        int lastMs = 0;


        end_ (AbstractController)
};

} // namespace GtkForms

#endif /* ABSTRACTCONTROLLER_H_ */
