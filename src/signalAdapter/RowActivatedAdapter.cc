/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "RowActivatedAdapter.h"
#include "Logging.h"
#include "mapping/GValueVariant.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get ();

Core::VariantVector RowActivatedAdapter::adapt (guint n_param_values, const GValue *param_values) const
{
        // Debug
        if (G_VALUE_TYPE (param_values + 1) == GTK_TYPE_TREE_PATH) {
                BOOST_LOG (lg) << "GTK_TREE_PATH, " << g_type_name (G_TYPE_FUNDAMENTAL(G_VALUE_TYPE (param_values + 1)));
        }

        if (G_VALUE_TYPE (param_values + 2) == GTK_TYPE_TREE_VIEW_COLUMN) {
                BOOST_LOG (lg) << "GTK_TYPE_TREE_VIEW_COLUMN, " << g_type_name (G_TYPE_FUNDAMENTAL(G_VALUE_TYPE (param_values + 2)));
        }

        for (unsigned int i = 0; i < n_param_values; ++i) {
                BOOST_LOG (lg) << "Parameter : " << G_VALUE_TYPE_NAME (param_values + i);
        }

        GValue const *gValue = param_values;
        if (!G_VALUE_HOLDS_OBJECT (gValue)) {
                throw Core::Exception ("GValue not object.");
        }

        GObject *object = G_OBJECT (g_value_get_object (gValue));
        GtkTreeView *treeView = GTK_TREE_VIEW (object);

        // Actual work:
        gValue = param_values + 1;
        if (!G_VALUE_HOLDS_BOXED (gValue)) {
                throw Core::Exception ("GValue not boxed.");
        }

        gpointer boxedPtr = g_value_get_boxed (gValue);
        GtkTreePath *path = static_cast <GtkTreePath *> (boxedPtr);

        gValue = param_values + 2;
        if (!G_VALUE_HOLDS_OBJECT (gValue)) {
                throw Core::Exception ("GValue not object.");
        }

        object = G_OBJECT (g_value_get_object (gValue));
        GtkTreeViewColumn *column = GTK_TREE_VIEW_COLUMN (object);

        // Find out which column number it is.
        guint numOfColumns = gtk_tree_view_get_n_columns (treeView);

        unsigned int activatedColumnNumber = 0;
        for (; activatedColumnNumber < numOfColumns; ++activatedColumnNumber) {
                GtkTreeViewColumn *c = gtk_tree_view_get_column (treeView, activatedColumnNumber);

                if (c == column) {
                        break;
                }
        }

        gchar *pathStr = gtk_tree_path_to_string (path);
        std::string pathStd = pathStr;
        g_free (pathStr);

        Core::VariantVector params;
        params.push_back (Core::Variant (treeView));
        params.push_back (Core::Variant (pathStd));
        params.push_back (Core::Variant (activatedColumnNumber));

        if (modelColumn != NO_MODEL_COLUMN) {
                GtkTreeModel *model = gtk_tree_view_get_model (treeView);

                if (!GTK_IS_TREE_MODEL (model)) {
                        throw Core::Exception ("RowActivatedAdapter::adapt : Could not conver treeViewModel to to GtkTreeModel.");
                }

                GtkTreeModel *treeModel = GTK_TREE_MODEL (model);
                GtkTreeIter iter;

                if (!gtk_tree_model_get_iter (treeModel, &iter, path)) {
                        throw Core::Exception ("RowActivatedAdapter::adapt : Something wrong went converting GtkTreePath to GtkTreeIter. The path is : [" + pathStd + "]");
                }

                GValue gVal = {0};
                gtk_tree_model_get_value (model, &iter, modelColumn, &gVal);
                params.push_back (gValueToVariant (&gVal));
        }

        return params;
}

} /* namespace GtkForms */
