/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_ABSTRACT_VIEW_H_
#define GTK_ABSTRACT_VIEW_H_

#include <string>
#include <map>
#include <set>
#include <gtk/gtk.h>
#include <core/Object.h>
#include <ReflectionParserAnnotation.h>
#include "mapping/Mapping.h"
#include "decorator/IPageDecorator.h"

namespace GtkForms {
class Context;
class AbstractController;
class AbstractAccessor;
struct Config;

/**
 * View (a top level windows, or some inner GtkWidget) created from GtkBuilder. Thise views are
 * loaded (i.e. memory is alloceted) in GtkAbstractView::show, and unloaded (memory is freed) in GtkAbstractView::hide (loadUi/destroyUi?).
 */
class __tiliae_reflect__  AbstractView : public Core::Object {
public:

        AbstractView ();
        virtual ~AbstractView ();

        std::string getName () const { return name; }
        void contId (std::string const &id) { name = id; }

        /*---------------------------------------------------------------------------*/

        static AbstractView *loadView (std::string const &viewAndSlot, AbstractController *controller, Ptr<Container::BeanFactoryContainer> container);

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

        typedef std::multimap <std::string, GtkWidget *> WidgetMap;
        WidgetMap getInputs (std::string const &dataRange, bool outputs = false);
        WidgetMap const &getSlots ();
        GtkWidget *getSlot (std::string const &name);
        void printStructure ();

        /*---------------------------------------------------------------------------*/

        MappingVector const &getMappings () const __tiliae_no_reflect__ { return mappings; }
        MappingMultiMap const &getMappingsByInput () const;
        MappingMultiMap getMappingsByModelRange (std::string const &modelRange) const;

        PageDecoratorVector &getDecorators () __tiliae_no_reflect__ { return decorators; }

        /*---------------------------------------------------------------------------*/

        virtual void connectSignals (AbstractAccessor *accessor) = 0;

        // TODO Should be protected
#define CONTROLLER_KEY "controller"
        static AbstractController *getControllerByWidget (GObject *widget);
        virtual AbstractController *getController ();
        virtual void setController (AbstractController *c);

        void setConfig (Config const *c);

public:

        std::string name;
        MappingVector mappings;
        PageDecoratorVector decorators;

protected:

        void populateInputMap ();
        void clearInternalState ();

private:

        struct Impl;
        Impl *impl = 0;
};

typedef __tiliae_reflect__ std::map <std::string, AbstractView *> ViewMap;
typedef std::set <AbstractView *> ViewSet;

} // namespace GtkForms

#endif /* GTKVIEW_H_ */
