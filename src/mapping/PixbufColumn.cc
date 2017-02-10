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

void PixbufColumn::setToView (ViewElementDTO *viewObject, std::string const &, Core::Variant valueToSet)
{
        ColumnElementDTO *colElem = dynamic_cast<ColumnElementDTO *> (viewObject);

        if (!colElem) {
                throw Core::Exception ("Column::setToView : dynamic_cast <ColumnElementDTO *> (viewObject) failed.");
        }

        GdkPixbuf *pixbuf = nullptr;
        GError *e = nullptr;

        if (getConstValue ().isNone ()) {
                std::string index = lcast<std::string> (valueToSet);

                AssociationMap::const_iterator it;
                if ((it = dict.find (index)) == dict.end ()) {
                        return;
                }

                pixbuf = gdk_pixbuf_new_from_file (it->second.c_str (), &e);
        }
        else {
                pixbuf = gdk_pixbuf_new_from_file (vcast<std::string> (getConstValue ()).c_str (), &e);
        }

        if (!pixbuf) {
                throw Core::Exception ("ConstantToPixbufMapping::model2View : failed to create GtkPixbuf from RawData. Message : [" + std::string (e->message)
                                       + "]");
        }

        if (colElem->listStore) {
                gtk_list_store_set (colElem->listStore, colElem->iter, colElem->columnNumber, pixbuf, -1);
        }
        else if (colElem->treeStore) {
                gtk_tree_store_set (colElem->treeStore, colElem->iter, colElem->columnNumber, pixbuf, -1);
        }
}

} // namespace
