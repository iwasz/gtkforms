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

        GtkView *getView () { return view; }
        std::string getName () const { return name; }
        SlotVector const &getSlots () const { return slots; }

        MappingVector const &getMappings () const { return mappings; };
        MappingMap const &getMappingsByInput () const;

        void loadUi (App *app);
        void destroyUi ();
        void reparent (Context *context);

        PageDecoratorVector &getDecorators () { return decorators; }

        friend std::ostream &operator<< (std::ostream &o, Page const &u);

private:

        mth_ (contId) void contId (std::string const &id) { name = id; }
        typedef std::map <std::string, GtkBin *> SlotWidgetMap;

        /**
         * Get slot widgets with thier names from GtkView and all GtkTiles attached to thid
         * page.
         */
        SlotWidgetMap getSlotWidgets ();
        void addToMapOrThrow (GObject *obj, Slot *slot, SlotWidgetMap *slotWidgets);

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
