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
        ColumnElementDTO *colElem = dynamic_cast<ColumnElementDTO *> (viewObject);

        if (!colElem) {
                throw Core::Exception ("Column::setToView : dynamic_cast <ColumnElementDTO *> (viewObject) failed.");
        }

        GValue gVal = G_VALUE_INIT;
        variantToGValue (&gVal, valueToSet);

        if (colElem->listStore) {
                gtk_list_store_set_value (colElem->listStore, colElem->iter, colElem->columnNumber, &gVal);
        }
        else if (colElem->treeStore) {
                gtk_tree_store_set_value (colElem->treeStore, colElem->iter, colElem->columnNumber, &gVal);
        }

        g_value_unset (&gVal);
}

} /* namespace GtkForms */
