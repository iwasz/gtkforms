/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Column.h"
#include "GValueVariant.h"

namespace GtkForms {

void Column::setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet)
{
        ColumnElementDTO *colElem = static_cast <ColumnElementDTO *> (viewObject);

        if (!GTK_IS_LIST_STORE (colElem->inputWidget)) {
                throw Core::Exception ("Column::setToView : Could not conver treeViewModel to to GtkListStore.");
        }

        GtkListStore *list = GTK_LIST_STORE (colElem->inputWidget);

        GValue gVal = G_VALUE_INIT;
        variantToGValue (&gVal, valueToSet);
        gtk_list_store_set_value (list, colElem->iter, colElem->columnNumber, &gVal);
        g_value_unset (&gVal);
}

} /* namespace GtkForms */
