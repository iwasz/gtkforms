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

//        Core::VariantMap &getSessionScope ();
//        Core::VariantMap &getUnitScope ();
//        Core::VariantMap &getFlashScope ();
        void setToSessionScope (std::string const &path, Core::Variant v);
        void setToUnitScope (std::string const &path, Core::Variant v);
        void setToFlashScope (std::string const &path, Core::Variant v);
        Core::Variant get (const std::string &name);

        virtual void onQuit () {}
        virtual void onIdle () {}
        virtual ValidationAndBindingResult validate () { return ValidationAndBindingResult {}; }

//        virtual std::string getName () const { return name; }
//        void setName (std::string const &name) { this->name = name; }

        SignalAdapterVector &getSignalAdapters () { return signalAdapters; }
        ValidatorVector const &getValidators () const { return validators; }

protected:

        App *app = 0;

private:

        void setApp (App *app) { this->app = app; }
//        mth_ (contId) void contId (std::string const &id) { name = id; }

private:

        ValidatorVector prr_ (validators);
        SignalAdapterVector prr_ (signalAdapters);



        end_ (AbstractController)
};

} // namespace GtkForms

#endif /* ABSTRACTCONTROLLER_H_ */
