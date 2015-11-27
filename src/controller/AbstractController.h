/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ABSTRACTCONTROLLER_H_
#define ABSTRACTCONTROLLER_H_

#include <string>
#include <vector>
#include <set>
#include <map>
#include <core/Object.h>
#include <ReflectionParserAnnotation.h>
#include <ReflectionParserAnnotation.h>
#include "signalAdapter/AbstractSignalAdapter.h"
#include "validator/IValidator.h"
#include "AbstractAccessor.h"

namespace GtkForms {

class AbstractView;
class AbstractController;
class App;
class AbstractAccessor;

/**
 * Vector of pointers to controllers.
 */
typedef std::vector <AbstractController *> ControllerVector;

/**
 * Set of pointers to controllers.
 */
typedef std::set <AbstractController *> ControllerSet;

/**
 * Map of pointers to controllers.
 */
typedef __tiliae_reflect__ std::map <std::string, AbstractController *> ControllerMap;

class __tiliae_reflect__ AbstractController : public Core::Object {
public:

        AbstractController ();
        virtual ~AbstractController ();

        void set (const std::string &name, Core::Variant data);
        Core::Variant get (const std::string &path);
        void setToSessionScope (std::string const &path, Core::Variant v);
        void clearControllerScope ();
        AbstractAccessor *getModelAccessor ();

        /*---------------------------------------------------------------------------*/

        void open (std::string const &childControllerNames);
        void openList (Core::StringVector const &childControllerNames);
        void closeThis ();
        void close (std::string const &childControllerNames);
        void closeList (Core::StringVector const &childControllerNames);
        void replace (std::string const &childControllerName);

        ControllerVector &getChildren () __tiliae_no_reflect__ ;
        ControllerVector const &getChildren () const __tiliae_no_reflect__;
        AbstractController *getParent () __tiliae_no_reflect__;
        AbstractController const *getParent () const __tiliae_no_reflect__;
        void setParent (AbstractController *c) __tiliae_no_reflect__;

        std::string getName () const;
        void contId (std::string const &id);

        AbstractController *findByName (std::string const &name);

        /*---------------------------------------------------------------------------*/

        virtual ValidationAndBindingResult validate () { return ValidationAndBindingResult {}; }

        SignalAdapterVector &getSignalAdapters () { return signalAdapters; }
        ValidatorVector const &getValidators () const { return validators; }

        ValidationAndBindingResultContainer const &getValidationResults () const __tiliae_no_reflect__;
        ValidationAndBindingResultContainer &getValidationResults () __tiliae_no_reflect__;

        /**
         * Do the opposite of submit i.e. converts data from model suitable to be presented on the
         * view, and shows it there.
         * \param dataRange Specifies which range of data to convert and display. Imagine you have
         * beefy table with tons of data. It would be pointless to convert all the contents if only
         * one tiny cell was changed. dataRange allows you to narrow the conversion process.
         */
        void refresh (std::string const &dataRange);

        /**
         * Users can request a submit using this action. It not perform any actions, only
         * sends an SubmitEvent to be processed in the next main loop iteration. doSubmit
         * performs all the work.
         * TODO Przeciążyć, żeby z jednym arg i bez arg.
         */
        void submit (std::string const &dataRange, std::string const &controllerName);

        /*---------------------------------------------------------------------------*/

        /**
         * Method which gets called one time on controller start. It may construct a model,
         * put it into one of scopes (session, controllerScope etc), and finally return the name of the
         * page to be shown. This method, should not be used by users.
         *
         * \returns Page to be shown or empty string in case nothing should be done with views.
         */
        virtual std::string onStart () = 0;

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
        virtual void onSubmit () = 0;

        /**
         * Gets called by managing objects when controller is about to be closed. Returns name
         * of the page to be hidden. This method, should not be used by users.
         *
         * \returns Page to be closed or empty string in case nothing should be done with views.
         */
        virtual void onStop () = 0;

        int getLoopDelayMs () const { return loopDelayMs; }
        int &getLastMs ();
        void performIdle (unsigned int currentMs);
        virtual void onQuit () {}

        /**
         * Gets called every getLoopDelayMs miliseconds.
         */
        virtual void onIdle () {}

        /*****************************************************************************/

        AbstractView *getView ();
        void setView (AbstractView *v);

        App *getApp ();

public:

        ValidatorVector validators;
        SignalAdapterVector signalAdapters;
        int loopDelayMs;

private:

        friend class App;
        void setApp (App *app);

        friend class HierarchicalAccessor;
        Core::VariantMap &getControllerScope ();

        struct Impl;
        Impl *impl;
};

} // namespace GtkForms

#endif /* ABSTRACTCONTROLLER_H_ */
