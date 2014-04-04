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
        if (!GTK_IS_TREE_VIEW (dto->viewElement->inputWidget)) {
                throw Core::Exception ("TableMapping::view2Model : Could not conver inputWidget to to GtkTreeView.");
        }

        GtkTreeView *treeView = GTK_TREE_VIEW (dto->viewElement->inputWidget);
        GtkTreeModel *model = gtk_tree_view_get_model (treeView);

        if (!model) {
                throw Core::Exception ("TableMapping::view2Model : GtkTreeModel is NULL in GtkTreeView.");
        }

        if (!GTK_IS_LIST_STORE (model)) {
                throw Core::Exception ("TableMapping::view2Model : Could not conver treeViewModel to to GtkListStore.");
        }

        GtkListStore *list = GTK_LIST_STORE (model);
        gtk_list_store_clear (list);
        modelColumnCopy.clear ();

        Wrapper::BeanWrapper *wrapper = dto->app->getBeanWrapper ();
        wrapper->setWrappedObject (dto->m2vModelObject);
        Ptr <IIterator> i = wrapper->iterator (modelCollection);
        GtkTreeIter iter;

        // Per row iteration.
        while (i->hasNext ()) {
                Variant element = i->next ();

                MappingDTO columnDTO;
                columnDTO.app = dto->app;
                columnDTO.m2vModelObject = element;
                columnDTO.v2mModelObject = element;
                columnDTO.dataRange = "";

                // Dodaj wiersz i uzyskaj iterator.
                gtk_list_store_append (list, &iter);

                ColumnElementDTO elementDTO;
                elementDTO.columnNumber = 0;
                elementDTO.inputWidget = G_OBJECT (list);
                elementDTO.iter = &iter;
                columnDTO.viewElement = &elementDTO;

                for (Column *column : columns) {

                        if (column->getModel ().empty ()) {
                                /*
                                 * This copy prevents deletion of smart_pointers. Rationale : TableMapping converts between some
                                 * custom collection (lets call it model-collection) of objects (lets call them modl-objects.
                                 * Both collection and objects must be managed by Core::Reflection), and
                                 * GtkListStore. If one wish to include the whole "model-object" (this is the object which gets
                                 * converted into single row of GtkListStore), he adds a <Column> without model specified, which
                                 * means that pointer to the whole model-object has to be stored in a GtkListStore's column. So
                                 * far so good, but problem occur, when model collection contain model-objects in form of values
                                 * not pointers. If you get such object as such:
                                 *
                                 * Variant element = i->next ();
                                 *
                                 * you get the copy (in fact Variant hold a smart_pointer then) which gets destroyed when Variant is
                                 * destroyed. But in GtkListStore we can store only a pointer to custom "model-objects", so finally
                                 * we end up with invalid pointer. modelColumnCopy collection solves this by maintaining a copy of
                                 * all model objects. So point to remember : collections of values in your domain model will cause
                                 * additional memory consumption (in this particullar scenario).
                                 */
                                modelColumnCopy.push_back (element);
                                GValue gVal = G_VALUE_INIT;
                                GtkForms::variantToGValue (&gVal, element);
                                gtk_list_store_set_value (list, &iter, elementDTO.columnNumber, &gVal);
                                g_value_unset (&gVal);
                        }
                        else {
                                column->model2View (&columnDTO);
                        }

                        ++(elementDTO.columnNumber);
                }
        }
}

} /* namespace GtkForms */