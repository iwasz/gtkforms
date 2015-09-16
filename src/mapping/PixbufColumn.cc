/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "PixbufColumn.h"
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "RawData.h"
#include "Context.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get();

void PixbufColumn::setToView (ViewElementDTO *viewObject, std::string const &, Core::Variant valueToSet)
{
        ColumnElementDTO *colElem = static_cast <ColumnElementDTO *> (viewObject);

        if (!GTK_IS_LIST_STORE (colElem->inputWidget)) {
                throw Core::Exception ("Column::setToView : Could not conver treeViewModel to to GtkListStore.");
        }

        GtkListStore *list = GTK_LIST_STORE (colElem->inputWidget);
        std::string index = lcast <std::string> (valueToSet);

        AssociationMap::const_iterator it;
        if ((it = dict.find (index)) == dict.end ()) {
                return;
        }

        GError *e = nullptr;
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file (it->second.c_str (), &e);

        if (!pixbuf) {
                throw Core::Exception ("ConstantToPixbufMapping::model2View : failed to create GtkPixbuf from RawData. Message : [" + std::string (e->message) + "]");
        }

        gtk_list_store_set (list, colElem->iter, colElem->columnNumber, pixbuf, -1);
}

} //namespace
