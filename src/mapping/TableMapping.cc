/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "TableMapping.h"
#include "App.h"
#include "Context.h"
#include "GValueVariant.h"
#include "Logging.h"
#include <Tiliae.h>
#include <gtk/gtk.h>

namespace GtkForms {
static src::logger_mt &lg = logger::get ();
using namespace Core;

ValidationAndBindingResult TableMapping::view2Model (MappingDTO *dto, std::string const &widgetName, std::string const &propertyName,
                                                     std::string const &modelName, Editor::IEditor *editor)
{
        // TODO
        return ValidationAndBindingResult{};
}

/*--------------------------------------------------------------------------*/

void TableMapping::model2View (MappingDTO *dto, std::string const &widgetName, std::string const &propertyName, std::string const &modelName,
                               Editor::IEditor *editor)
{
        if (!GTK_IS_TREE_VIEW (dto->viewElement->inputWidget)) {
                throw Core::Exception ("TableMapping::model2View : Could not conver inputWidget to to GtkTreeView.");
        }

        GtkTreeView *treeView = GTK_TREE_VIEW (dto->viewElement->inputWidget);
        GtkTreeModel *treeModel = gtk_tree_view_get_model (treeView);

        if (!treeModel) {
                throw Core::Exception ("TableMapping::model2View : GtkTreeModel is NULL in GtkTreeView.");
        }

        GtkListStore *listStore = nullptr;
        GtkTreeStore *treeStore = nullptr;
        extractModels (treeModel, &listStore, &treeStore);

        if (listStore) {
                gtk_list_store_clear (listStore);
        }
        else if (treeStore && !dynamic_cast<ColumnElementDTO *> (dto->viewElement)) {
                gtk_tree_store_clear (treeStore);
        }

        modelColumnCopy.clear ();

        Wrapper::BeanWrapper *wrapper = dto->app->getBeanWrapper ();
        wrapper->setWrappedObject (dto->m2vModelObject);
        Ptr<IIterator> i = wrapper->iterator (modelName);
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
                if (listStore) {
                        gtk_list_store_append (listStore, &iter);
                }
                else if (treeStore) {
                        ColumnElementDTO *parentElementDTO = dynamic_cast<ColumnElementDTO *> (dto->viewElement);

                        if (!parentElementDTO) {
                                gtk_tree_store_append (treeStore, &iter, nullptr);
                        }
                        else {
                                gtk_tree_store_append (treeStore, &iter, parentElementDTO->iter);
                        }
                }

                ColumnElementDTO elementDTO;
                elementDTO.columnNumber = 0;
                elementDTO.inputWidget = dto->viewElement->inputWidget;
                elementDTO.listStore = listStore;
                elementDTO.treeStore = treeStore;
                elementDTO.iter = &iter;
                columnDTO.viewElement = &elementDTO;

                for (IMapping *column : columns) {

                        Column *c = dynamic_cast<Column *> (column);
                        if (c && c->isStoreThisRow ()) {
                                /*
                                 * This copy prevents deletion of smart_pointers. Rationale : TableMapping converts between some
                                 * custom collection (lets call it model-collection) of objects (lets call them modl-objects.
                                 * Both collection and objects must be managed by Core::Reflection), and
                                 * GtkListStore. If one wish to include the whole "model-object" (this is the object which gets
                                 * converted into single row of GtkListStore), he adds a <Column> without model specified, which
                                 * means that pointer to the whole model-object has to be stored in a GtkListStore's column. So
                                 * far so good, but problem occurs, when model collection contain model-objects in form of values
                                 * not pointers. If you get such object as so:
                                 *
                                 * Variant element = i->next ();
                                 *
                                 * you get a copy (in fact Variant hold a smart_pointer then) which gets destroyed when Variant is
                                 * destroyed. But in GtkListStore we can store only a pointer to custom "model-objects", so finally
                                 * we end up with invalid pointer. modelColumnCopy collection solves this by maintaining a copy of
                                 * all model objects. So point to remember : collections of values in your domain model will cause
                                 * additional memory consumption (in this particullar scenario).
                                 */
                                modelColumnCopy.push_back (element);
                                GValue gVal = G_VALUE_INIT;
                                GtkForms::variantToGValue (&gVal, element);

                                if (listStore) {
                                        gtk_list_store_set_value (listStore, &iter, elementDTO.columnNumber, &gVal);
                                }
                                else if (treeStore) {
                                        gtk_tree_store_set_value (treeStore, &iter, elementDTO.columnNumber, &gVal);
                                }

                                g_value_unset (&gVal);
                        }

                        // If Column::model is empty, we skip to another column both in Collumns, and in GtkListStore.
                        if (!column->getModel ().empty () || !column->getConstValue ().isNone ()) {
                                column->model2View (&columnDTO);
                        }

                        ++(elementDTO.columnNumber);
                }
        }
}

/*****************************************************************************/

void TableMapping::extractModels (GtkTreeModel *treeModel, GtkListStore **listStore, GtkTreeStore **treeStore)
{
        if (GTK_IS_TREE_MODEL_FILTER (treeModel)) {
                extractModels (gtk_tree_model_filter_get_model (GTK_TREE_MODEL_FILTER (treeModel)), listStore, treeStore);
        }
        else if (GTK_IS_TREE_MODEL_SORT (treeModel)) {
                extractModels (gtk_tree_model_sort_get_model (GTK_TREE_MODEL_SORT (treeModel)), listStore, treeStore);
        }
        else if (GTK_IS_LIST_STORE (treeModel)) {
                *listStore = GTK_LIST_STORE (treeModel);
        }
        else if (GTK_IS_TREE_STORE (treeModel)) {
                *treeStore = GTK_TREE_STORE (treeModel);
        }
        else {
                throw Core::Exception ("TableMapping::model2View : Could not conver treeViewModel to to GtkListStore.");
        }
}

} /* namespace GtkForms */
