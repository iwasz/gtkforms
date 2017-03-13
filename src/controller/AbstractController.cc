/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "AbstractController.h"
#include "App.h"
#include "Context.h"
#include "HierarchicalAccessor.h"
#include "RefreshEvent.h"
#include "SubmitEvent.h"
#include <boost/algorithm/string/split.hpp>

namespace GtkForms {

struct AbstractController::Impl {
        int lastMs = 0;
        Core::VariantMap controllerScope;
        ValidationAndBindingResultContainer validationResults;
        AbstractController *parent = nullptr;
        ControllerVector children;
        std::string name;
        // AbstractView *view = nullptr;
        ViewVector views;
        App *app = nullptr;
        HierarchicalAccessor accessor;
};

/*****************************************************************************/

AbstractController::AbstractController () : loopDelayMs{ -1 }
{
        impl = new Impl;
        impl->accessor.setCurrentController (this);
}

/*****************************************************************************/

AbstractController::~AbstractController () { delete impl; }

/*****************************************************************************/

void AbstractController::setApp (App *app)
{
        impl->app = app;

        if (app) {
                impl->accessor.setSessionScope (&app->getContext ().getSessionScope ());
        }
}

/*****************************************************************************/

App *AbstractController::getApp () { return impl->app; }

/*---------------------------------------------------------------------------*/

void AbstractController::set (std::string const &path, Core::Variant data)
{
        //        impl->app->getContext ().setCurrentController (this);
        impl->app->getContext ().setToControllerScope (&impl->controllerScope, path, data);
}

/*---------------------------------------------------------------------------*/

void AbstractController::setToSessionScope (std::string const &path, Core::Variant data) { impl->app->getContext ().setToSessionScope (path, data); }

/*---------------------------------------------------------------------------*/

/*
 * Warning, IoC container crashes here, when non existent property was specified (it falls back
 * to generic get/set methods which were not meant to be used for that).
 */
Core::Variant AbstractController::get (const std::string &path)
{
        Wrapper::BeanWrapper *wrapper = impl->app->getBeanWrapper ();
        wrapper->setWrappedObject (Core::Variant (&impl->accessor));
        Core::DebugContext ctx;
        bool error = false;
        Core::Variant v = wrapper->get (path, &error, &ctx);

        if (error) {
                Core::Exception e ("Context::get failed. name = [" + path + "]");
                e.addContext (ctx);
                throw e;
        }

        return v;
}

/*---------------------------------------------------------------------------*/

void AbstractController::clearControllerScope () { impl->controllerScope.clear (); }

/*****************************************************************************/

void AbstractController::open (std::string const &childControllerNames)
{
        Core::StringVector namesList;
        boost::split (namesList, childControllerNames, boost::is_any_of (", "), boost::token_compress_on);
        openList (namesList);
}

/*---------------------------------------------------------------------------*/

void AbstractController::openList (Core::StringVector const &childControllerNames) { impl->app->open (this, childControllerNames); }

/*---------------------------------------------------------------------------*/

void AbstractController::closeThis () { impl->app->close (this); }

/*---------------------------------------------------------------------------*/

void AbstractController::close (std::string const &childControllerNames)
{
        Core::StringVector namesList;
        boost::split (namesList, childControllerNames, boost::is_any_of (", "), boost::token_compress_on);
        impl->app->close (this, namesList);
}

/*---------------------------------------------------------------------------*/

void AbstractController::closeList (Core::StringVector const &childControllerNames) { impl->app->close (this, childControllerNames); }

/*---------------------------------------------------------------------------*/

void AbstractController::replace (std::string const &childControllerName)
{
        closeThis ();
        impl->app->open (getParent (), childControllerName);
}

/*****************************************************************************/

void AbstractController::performIdle (unsigned int currentMs)
{
        if (getLoopDelayMs () > 0) {
                if (getLastMs () + getLoopDelayMs () <= currentMs) {
                        getLastMs () = currentMs;
                        //                        impl->context.setCurrentController (controller);
                        onIdle ();
                }
        }

        for (AbstractController *child : impl->children) {
                child->performIdle (currentMs);
        }
}

/*****************************************************************************/

AbstractController *AbstractController::findByName (std::string const &name)
{
        if (getName () == name) {
                return this;
        }

        for (AbstractController *child : impl->children) {
                AbstractController *found;
                if ((found = child->findByName (name))) {
                        return found;
                }
        }

        return nullptr;
}

/*****************************************************************************/

void AbstractController::refresh (std::string const &widgetNameRange, std::string const &propertyName)
{
        RefreshEvent *event = new RefreshEvent;
        event->controller = this;
        event->widgetNameRange = widgetNameRange;
        event->propertyName = propertyName;
        std::unique_ptr<IEvent> e{ event };
        impl->app->pushEvent (std::move (e));
}

/*****************************************************************************/

void AbstractController::submit (std::string const &widgetNameRange, std::string const &controllerName)
{
        SubmitEvent *event = new SubmitEvent;
        event->controller = this;
        event->widgetNameRange = widgetNameRange;
        event->controllerName = controllerName;
        std::unique_ptr<SubmitEvent> e{ event };
        impl->app->pushEvent (std::move (e));
}

/*****************************************************************************/

ControllerVector &AbstractController::getChildren () { return impl->children; }

ControllerVector const &AbstractController::getChildren () const { return impl->children; }

AbstractController *AbstractController::getParent () { return impl->parent; }

AbstractController const *AbstractController::getParent () const { return impl->parent; }

void AbstractController::setParent (AbstractController *c) { impl->parent = c; }

std::string AbstractController::getName () const { return impl->name; }

void AbstractController::contId (std::string const &id) { impl->name = id; }

ValidationAndBindingResultContainer const &AbstractController::getValidationResults () const { return impl->validationResults; }

ValidationAndBindingResultContainer &AbstractController::getValidationResults () { return impl->validationResults; }

int &AbstractController::getLastMs () { return impl->lastMs; }

ViewVector const &AbstractController::getViews () { return impl->views; }

// void AbstractController::setView (AbstractView *v) { impl->view = v; }
void AbstractController::addView (AbstractView *v) { impl->views.push_back (v); }
void AbstractController::clearViews () { impl->views.clear (); }

Core::VariantMap &AbstractController::getControllerScope () { return impl->controllerScope; }

AbstractAccessor *AbstractController::getModelAccessor () { return &impl->accessor; }

} // namespace GtkForms
