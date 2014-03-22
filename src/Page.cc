/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Page.h"

namespace GtkForms {
using namespace std;

Page::~Page ()
{
        delete mappingsByInputCache;
        delete mappingsByModelCache;
}

/*--------------------------------------------------------------------------*/

void Page::loadUi (App *app)
{
        if (view) {
                view->loadUi (app);
        }

        for (Slot *s : slots) {
                GtkTile *tile = s->getTile ();

                if (tile) {
                        tile->loadUi (app);
                }
        }
}

/*--------------------------------------------------------------------------*/

void Page::destroyUi ()
{
        for (Slot *s : slots) {
                GtkTile *tile = s->getTile ();
                tile->destroyUi ();
        }

        if (view) {
                view->destroyUi ();
        }
}

/*--------------------------------------------------------------------------*/

ostream &operator<< (ostream &o, Page const &p)
{
        o << "Page [view : [" << p.view->getName () << "], tiles : [";

        for (auto i = p.slots.begin (); i != p.slots.end (); ) {
                GtkTile *tile = (*i)->getTile ();
                o << tile->getName ();

                if (++i != p.slots.end ()) {
                        o << ", ";
                }
        }

        o << "]]";
        return o;
}

/*--------------------------------------------------------------------------*/

MappingMap const &Page::getMappingsByInput () const
{
        if (mappingsByInputCache) {
                return *mappingsByInputCache;
        }

        mappingsByInputCache = new MappingMap;

        for (IMapping *mapping : mappings) {
                mappingsByInputCache->operator[] (mapping->getInput()) = mapping;
        }

        return *mappingsByInputCache;
}

/*--------------------------------------------------------------------------*/

//MappingMap const &Page::getMappingsByModel () const
//{
//        if (mappingsByModelCache) {
//                return *mappingsByModelCache;
//        }
//
//        mappingsByModelCache = new MappingMap;
//
//        for (IMapping *mapping : mappings) {
//                mappingsByModelCache->operator[] (mapping->getModel ()) = mapping;
//        }
//
//        return *mappingsByModelCache;
//}

} // namespace GtkForms
