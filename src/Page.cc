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

void Page::loadUi (Context *context)
{
        if (view) {
                view->loadUi (context);
        }

        for (auto i : tiles) {
                i.second->loadUi (context);
        }
}

/*--------------------------------------------------------------------------*/

void Page::destroyUi ()
{
        for (auto elem : tiles) {
                GtkTile *tile = elem.second;
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

        for (auto i = p.tiles.begin (); i != p.tiles.end (); ) {
                o << i->second->getName ();

                if (++i != p.tiles.end ()) {
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
