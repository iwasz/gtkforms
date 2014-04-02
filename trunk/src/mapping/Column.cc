/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Column.h"

namespace GtkForms {

void Column::setToView (GObject *viewObject, std::string const &finalProperty, Core::Variant valueToSet)
{
        if (!GTK_IS_LIST_STORE (viewObject)) {
                throw Core::Exception ("Column::setToView : Could not conver treeViewModel to to GtkListStore.");
        }

        GtkListStore *list = GTK_LIST_STORE (viewObject);


        GValue gVal = G_VALUE_INIT;
        variantToGValue (&gVal, output);
        gtk_list_store_set_value (list, &iter, colNo++, &gVal);
        g_value_unset (&gVal);
}

} /* namespace GtkForms */
