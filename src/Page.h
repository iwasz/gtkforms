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
        REFLECTION_CONSTRUCTOR_ (void)

        virtual ~Page ();

        GtkView *getView () { return view; }
        std::string getName () const { return name; }
        SlotVector const &getSlots () const { return slots; }

        MappingVector const &getMappings () const { return mappings; };
        MappingMultiMap const &getMappingsByInput () const;
        MappingMultiMap getMappingsByModelRange (std::string const &modelRange) const;

        virtual void loadUi (App *app);
        virtual void destroyUi ();
        virtual void show ();
        virtual void hide ();
        virtual void reparent (Context *context);

        virtual GObject *getUiOrThrow (std::string const &name);
        virtual GObject *getUi (std::string const &name);

        PageDecoratorVector &getDecorators () { return decorators; }

        friend std::ostream &operator<< (std::ostream &o, Page const &u);

protected:

        REFLECTION_METHOD (contId) void contId (std::string const &id) { name = id; }
        typedef std::map <std::string, GtkBin *> SlotWidgetMap;

        /**
         * Get slot widgets with thier names from GtkView and all GtkTiles attached to thid
         * page.
         */
        virtual SlotWidgetMap getSlotWidgets ();
        virtual void addToMapOrThrow (GObject *obj, Slot *slot, SlotWidgetMap *slotWidgets);

private:

        GtkView *REFLECTION_FIELD_VALUE_INPLACE (view);
        std::string REFLECTION_FIELD_REFERENCE_INPLACE (name);
        SlotVector REFLECTION_FIELD_REFERENCE_INPLACE (slots);
        MappingVector REFLECTION_FIELD_REFERENCE_INPLACE (mappings);
        mutable MappingMultiMap *mappingsByInputCache = 0;
//        mutable MappingMap *mappingsByModelCache = 0;
        PageDecoratorVector REFLECTION_FIELD_REFERENCE_INPLACE (decorators);
        REFLECTION_END (Page)
};

typedef std::map <std::string, Page *> PageMap;
typedef std::set <Page *> PageSet;

} // namespace GtkForms

#endif /* PAGE_H_ */
