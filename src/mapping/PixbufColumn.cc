/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "PixbufColumn.h"
#include "Context.h"
#include "RawData.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>

namespace GtkForms {
static src::logger_mt &lg = logger::get ();

void PixbufColumn::init ()
{
        GError *e = nullptr;
        GdkPixbuf *pixbuf;

        for (AssociationMap::value_type const &el : dict) {
                std::string key = el.first;
                std::string val = el.second;

                pixbuf = gdk_pixbuf_new_from_file (val.c_str (), &e);

                if (!pixbuf) {
                        throw Core::Exception ("PixbufColumn::init : failed to create GtkPixbuf from a file [" + val + "]. Message : ["
                                               + std::string (e->message) + "]");
                }

                cache[key] = pixbuf;
        }

        if (!getConstValue ().isNone ()) {
                std::string path = vcast<std::string> (getConstValue ());
                pixbuf = gdk_pixbuf_new_from_file (vcast<std::string> (getConstValue ()).c_str (), &e);

                if (!pixbuf) {
                        throw Core::Exception ("PixbufColumn::init : failed to create GtkPixbuf from a constValue [" + path + "]. Message : ["
                                               + std::string (e->message) + "]");
                }

                cache[path] = pixbuf;
        }
}

/*****************************************************************************/

PixbufColumn::~PixbufColumn ()
{
        for (PixBufMap::value_type &el : cache) {
                g_object_unref (el.second);
        }
}

/*****************************************************************************/

void PixbufColumn::setToView (ViewElementDTO *viewObject, std::string const &, Core::Variant valueToSet)
{
        ColumnElementDTO *colElem = dynamic_cast<ColumnElementDTO *> (viewObject);

        if (!colElem) {
                throw Core::Exception ("Column::setToView : dynamic_cast <ColumnElementDTO *> (viewObject) failed.");
        }

        GdkPixbuf *pixbuf = nullptr;

        if (getConstValue ().isNone ()) {
                pixbuf = cache.at (lcast<std::string> (valueToSet));
        }
        else {
                pixbuf = cache.at (vcast<std::string> (getConstValue ()));
        }

        if (colElem->listStore) {
                gtk_list_store_set (colElem->listStore, colElem->iter, colElem->columnNumber, pixbuf, -1);
        }
        else if (colElem->treeStore) {
                gtk_tree_store_set (colElem->treeStore, colElem->iter, colElem->columnNumber, pixbuf, -1);
        }
}

} // namespace
