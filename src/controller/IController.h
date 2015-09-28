/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ICONTROLLER_H_
#define ICONTROLLER_H_

#include <string>
#include <vector>
#include <set>
#include <map>
#include <core/Object.h>
#include <ReflectionParserAnnotation.h>
#include "signalAdapter/AbstractSignalAdapter.h"
#include "validator/IValidator.h"

namespace GtkForms {

class App;

/*
 * A controller. Controllers do so called business tasks as opposed to views from which they are
 * separated. Controller is a single unit of work, and performs operations on model data which
 * can be obtained from a database or from user input. Teher is always at least one controller
 * running, but there may be more of them as well. In such case they form a stacked structure
 * where controller created first is called a "root controller" and the rest is called "child
 * controllers".
 */
struct __tiliae_reflect__ IController : public Core::Object {

        /**
         * Method which gets called one time on controller start. It may construct a model,
         * put it into one of scopes (session, flash etc), and finally return the name of the
         * page to be shown. This method, should not be used by users.
         *
         * \returns Page to be shown or empty string in case nothing should be done with views.
         */
        virtual std::string start () = 0;

        /**
         * This method is called whenever a view submits some data (model). Usually in MVC
         * environments such data is called a "form". By invoking this method (not directly)
         * view informs the controller that user has input some data, and controller should
         * pay attention and perform some action on it. For example if it were to be login
         * screen, this method would check for password validity and depending on the result
         * it would return new view name (in case of successful login), or this controller own
         * name (as configured in XML) plus some warning message. This method, should not be
         * used by users.
         *
         * \return View name to be displayed or empty string in case nothing to be done with
         * views.
         */
        virtual std::string onSubmit () = 0;
        virtual void onQuit () = 0;

        /**
         * Gets called every getLoopDelayMs miliseconds.
         */
        virtual void onIdle () = 0;
        virtual ValidationAndBindingResult validate () = 0;

        /**
         * Gets called by managing objects when controller is about to be closed. Returns name
         * of the page to be hidden. This method, should not be used by users.
         *
         * \returns Page to be closed or empty string in case nothing should be done with views.
         */
        virtual std::string end () = 0;

        /**
         * Controller may access the App object which enables them to navigate to some other unit
         * (by invoking getApp ()->show ("unit")), or add some new unit to themselves (by invoking
         * getApp()->merge ()) among other actions like back, forward and quit.
         */
        virtual App *getApp () = 0;

        virtual void setToSessionScope (std::string const &path, Core::Variant v) = 0;
        virtual void setToUnitScope (std::string const &path, Core::Variant v) = 0;
        virtual void setToFlashScope (std::string const &path, Core::Variant v) = 0;
        virtual Core::VariantMap &getFlashScope () = 0;
        virtual void clearFlashScope () = 0;
        virtual Core::Variant get (const std::string &name) = 0;

        virtual SignalAdapterVector &getSignalAdapters () = 0;
        virtual ValidatorVector const &getValidators () const = 0;

        virtual int getLoopDelayMs () const = 0;
        virtual int &getLastMs () = 0;

        virtual ValidationAndBindingResultContainer const &getValidationResults () const = 0;
        virtual ValidationAndBindingResultContainer &getValidationResults () = 0;

protected:

        friend class App;
        virtual void setApp (App *app) = 0;

};

/**
 * Vector of pointers to controllers.
 */
typedef std::vector <IController *> ControllerVector;

/**
 * Set of pointers to controllers.
 */
typedef std::set <IController *> ControllerSet;

/**
 * Map of pointers to controllers.
 */
typedef std::map <std::string, IController *> ControllerMap;
REFLECTION_COLLECTION (ControllerMap)

} // namespace GtkForms

#endif /* ICONTROLLER_H_ */
