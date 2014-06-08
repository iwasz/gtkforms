/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_PAGE_H_
#define GTK_FORMS_PAGE_H_

#include <ostream>
#include <set>
#include "decorator/IPageDecorator.h"
#include "view/IView.h"
#include "mapping/Mapping.h"
#include "view/GtkTile.h"
#include "view/GtkView.h"
#include "view/Slot.h"

namespace GtkForms {

class Page : public Core::Object {
public:
        ctr__ (void)

        virtual ~Page ();

        virtual GtkView *getView () { return view; }
        virtual std::string getName () const { return name; }
        virtual SlotVector const &getSlots () const { return slots; }

        virtual MappingVector const &getMappings () const { return mappings; };
        virtual MappingMap const &getMappingsByInput () const;

        virtual void loadUi (App *app);
        virtual void destroyUi ();
        virtual void show ();
        virtual void hide ();
        virtual void reparent (Context *context);

        virtual GObject *getUiOrThrow (std::string const &name);
        virtual GObject *getUi (std::string const &name);

        virtual PageDecoratorVector &getDecorators () { return decorators; }

        friend std::ostream &operator<< (std::ostream &o, Page const &u);

protected:

        mth_ (contId) void contId (std::string const &id) { name = id; }
        typedef std::map <std::string, GtkBin *> SlotWidgetMap;

        /**
         * Get slot widgets with thier names from GtkView and all GtkTiles attached to thid
         * page.
         */
        virtual SlotWidgetMap getSlotWidgets ();
        virtual void addToMapOrThrow (GObject *obj, Slot *slot, SlotWidgetMap *slotWidgets);

private:

        GtkView *prp_ (view);
        std::string prr_ (name);
        SlotVector prr_ (slots);
        MappingVector prr_ (mappings);
        mutable MappingMap *mappingsByInputCache = 0;
        mutable MappingMap *mappingsByModelCache = 0;
        PageDecoratorVector prr_ (decorators);
        end_ (Page)
};

typedef std::map <std::string, Page *> PageMap;
typedef std::set <Page *> PageSet;

} // namespace GtkForms

#endif /* PAGE_H_ */
