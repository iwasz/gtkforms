/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_ABSTRACT_VIEW_H_
#define GTK_ABSTRACT_VIEW_H_

#include "controller/ViewsToOpen.h"
#include "decorator/IPageDecorator.h"
#include "mapping/Mapping.h"
#include "signalAdapter/AbstractSignalAdapter.h"
#include <ReflectionParserAnnotation.h>
#include <core/Object.h>
#include <gtk/gtk.h>
#include <map>
#include <string>
#include <vector>

namespace GtkForms {
class Context;
class AbstractController;
class AbstractAccessor;
struct Config;

/**
 * View (a top level windows, or some inner GtkWidget) created from GtkBuilder. Thise views are
 * loaded (i.e. memory is alloceted) in GtkAbstractView::show, and unloaded (memory is freed) in GtkAbstractView::hide (loadUi/destroyUi?).
 */
class __tiliae_reflect__ AbstractView : public Core::Object {
public:
        AbstractView ();
        virtual ~AbstractView ();

        std::string getName () const { return name; }
        void contId (std::string const &id) { name = id; }

        /*---------------------------------------------------------------------------*/

        static AbstractView *loadView (ViewsToOpen::ViewSlot const &vs, AbstractController *controller, Container::BeanFactoryContainer *container);

        /**
         * Loads a UI file into memory (if not loaded allready), and gets pointer to widget with name
         * name. If widget was obtained from the file earlier, it will be returned immediately, and no
         * new instance of this widget will be made.
         */
        virtual void loadUi (App *app) = 0;
        virtual void show ();
        virtual void hide ();
        virtual void refresh (Context *) {}
        virtual bool reparent (std::string const &slotName);
//        virtual bool embed (std::string const &slotName);

        /**
         * Destroys
         */
        virtual void destroyUi () = 0;

        /**
         * Gets the main object (GTK+ GObject) from the ui file.
         */
        virtual GObject *getUi () __tiliae_no_reflect__ = 0;
        virtual bool isLoaded () const = 0;

        /**
         * Get arbitrary object from UI.
         */
        virtual GObject *getUiOrThrow (std::string const &name);
        virtual GObject *getUi (std::string const &name) __tiliae_no_reflect__ = 0;

        typedef std::multimap<std::string, GtkWidget *> WidgetMap;

        /// Get some input/outpu widgets by name.
        WidgetMap getInputOutputWidgets (std::string const &widgetNameRange, bool outputs = false);
        WidgetMap const &getSlots ();
        GtkWidget *getSlot (std::string const &name);
        void printStructure ();

        /*---------------------------------------------------------------------------*/

        MappingVector const &getMappings () const __tiliae_no_reflect__ { return mappings; }
        MappingMultiMap getMappingsByInputRange (std::string const &widgetNameRange = "") const;
        MappingMultiMap getMappingsByModelRange (std::string const &modelRange) const;

        PageDecoratorVector &getDecorators () __tiliae_no_reflect__ { return decorators; }
        virtual void runDecorators (IPageDecorator::Stage stage, Context *ctx);

        /*---------------------------------------------------------------------------*/

        virtual void connectSignals (AbstractAccessor *accessor) = 0;

        SignalAdapterVector &getSignalAdapters () { return signalAdapters; }

// TODO Should be protected
#define CONTROLLER_KEY "controller"
        static AbstractController *getControllerByWidget (GObject *widget);
        /// Gets the controller pointer from the GTK widget
        virtual AbstractController *getControllerFromUi ();
        /// Adds controller pointer to the GTK widget
        virtual void setControllerToUi (AbstractController *c);

        /// Gets the controller pointer from class field
        virtual AbstractController *getController ();
        /// Adds controller pointer to a class field
        virtual void setController (AbstractController *c);

        void setConfig (Config const *c);

public:
        std::string name;
        MappingVector mappings;
        PageDecoratorVector decorators;
        SignalAdapterVector signalAdapters;

protected:
        void populateInputMap ();
        void clearInternalState ();

private:
        struct Impl;
        Impl *impl = 0;
};

// typedef __tiliae_reflect__ std::map<std::string, AbstractView *> ViewMap;
// typedef std::set<AbstractView *> ViewSet;
typedef std::vector<AbstractView *> ViewVector;

} // namespace GtkForms

#endif /* GTKVIEW_H_ */
