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

        Core::VariantMap &getSessionScope ();
        Core::VariantMap &getUnitScope ();
        Core::VariantMap &getFlashScope ();

        virtual void onQuit () {}
        virtual void onIdle () {}

        virtual std::string getName () const { return name; }
        mth_ (setName) void setName (std::string const &name) { this->name = name; }

private:

        void setApp (App *app) { this->app = app; }

private:
        App *app = 0;
        std::string name;

        end_ (AbstractController)
};

} // namespace GtkForms

#endif /* ABSTRACTCONTROLLER_H_ */
