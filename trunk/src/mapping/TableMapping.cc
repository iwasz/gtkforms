/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "TableMapping.h"
#include "Logging.h"
#include "App.h"
#include "Context.h"
#include <gtk/gtk.h>
#include <Tiliae.h>
#include "GValueVariant.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get();
using namespace Core;

void TableMapping::view2Model (MappingDTO *dto)
{
}

/*--------------------------------------------------------------------------*/

void TableMapping::model2View (MappingDTO *dto)
{
        if (!GTK_IS_TREE_VIEW (dto->inputWidget)) {
                throw Core::Exception ("TableMapping::view2Model : Could not conver inputWidget to to GtkTreeView.");
        }

        GtkTreeView *treeView = GTK_TREE_VIEW (dto->inputWidget);
        GtkTreeModel *model = gtk_tree_view_get_model (treeView);

        if (!GTK_IS_LIST_STORE (model)) {
                throw Core::Exception ("TableMapping::view2Model : Could not conver treeViewModel to to GtkListStore.");
        }

        GtkListStore *list = GTK_LIST_STORE (model);

        Wrapper::BeanWrapper *wrapper = dto->app->getBeanWrapper ();
        Core::VariantMap &unitScope = dto->context->getUnitScope ();
        wrapper->setWrappedObject (Core::Variant (&unitScope));

        Ptr <IIterator> i = wrapper->iterator (modelCollection);

        GtkTreeIter iter;

        while (i->hasNext ()) {

                // Wiersz kolecji wejściowej - najlepiej, żeby to była lista, wektor, lub set.
                Variant element = i->next ();
                wrapper->setWrappedObject (element);

                // Dodaj wiersz i uzyskaj iterator.
                gtk_list_store_append (list, &iter);

                /*
                 * TODO!!! koniecznie powinien tu pobrać listę kolumn z GtkListStore i spróbować pobrać obiekt Column.
                 * Jesli jest to OK, to używamy z niego informacji, ale jeśli nie ma, to powinien spróbować tak samo jak
                 * w przypadku normalnych inputów ustawić dane na pałkę.
                 */
                unsigned int colNo = 0;
                for (Column *column : columns) {
                        GValue gVal = G_VALUE_INIT;
                        Variant vVal = wrapper->get (&element, column->model);
                        GtkForms::variantToGValue (&gVal, vVal);
                        gtk_list_store_set_value (list, &iter, colNo++, &gVal);
                }
        }
}

} /* namespace GtkForms */
