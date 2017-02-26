/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "TableFilterMapping.h"
#include "Logging.h"
#include <gtk/gtk.h>
#include <string>

namespace GtkForms {
static src::logger_mt &lg = logger::get ();

struct TableFilterMapping::Impl {
        GtkTreeModelFilter *gtkTreeModelFilter = nullptr;
        //        std::string widget;
        //        std::string model;
        std::string query;
        int columnNumber = -1;

        static gboolean gtkTreeModelFilterVisibleFunc (GtkTreeModel *model, GtkTreeIter *iter, gpointer data);
};

/*****************************************************************************/

TableFilterMapping::TableFilterMapping () : impl (new Impl) {}

/*****************************************************************************/

TableFilterMapping::~TableFilterMapping () { delete impl; }

/*****************************************************************************/

// To powinna być metoda setToview i dziedziczymy z Mapping.
// Albo jeśli nie dziedziczymy z mapping, to to powinna być metoda model2View!

void TableFilterMapping::setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet)
{
        if (!GTK_IS_TREE_VIEW (viewObject->inputWidget)) {
                throw Core::Exception ("TableFilterMapping::view2Model : Could not conver inputWidget to to GtkTreeView.");
        }

        GtkTreeView *treeView = GTK_TREE_VIEW (viewObject->inputWidget);
        GtkTreeModel *treeModel = gtk_tree_view_get_model (treeView);

        if (!treeModel) {
                throw Core::Exception ("TableFilterMapping::view2Model : GtkTreeModel is NULL in GtkTreeView.");
        }

        if (impl->columnNumber < 0) {
                throw Core::Exception ("TableFilterMapping::view2Model : please set columnNumber first.");
        }

        if (!impl->gtkTreeModelFilter) {
                impl->gtkTreeModelFilter = GTK_TREE_MODEL_FILTER (gtk_tree_model_filter_new (treeModel, nullptr));
                gtk_tree_model_filter_set_visible_func (impl->gtkTreeModelFilter, &TableFilterMapping::Impl::gtkTreeModelFilterVisibleFunc, impl, nullptr);
                gtk_tree_view_set_model (treeView, GTK_TREE_MODEL (impl->gtkTreeModelFilter));
        }

        impl->query = vcast<std::string> (valueToSet);
        BOOST_LOG (lg) << "++++ [" << impl->query << "]";
        gtk_tree_model_filter_refilter (impl->gtkTreeModelFilter);
}

/*****************************************************************************/

int TableFilterMapping::getColumnNumber () const { return impl->columnNumber; }
void TableFilterMapping::setColumnNumber (int value) { impl->columnNumber = value; }

/*****************************************************************************/

gboolean TableFilterMapping::Impl::gtkTreeModelFilterVisibleFunc (GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
        TableFilterMapping::Impl *impl = static_cast<TableFilterMapping::Impl *> (data);

        GValue gVal = { 0 };
        gtk_tree_model_get_value (model, iter, impl->columnNumber, &gVal);
        const gchar *gStr = g_value_get_string (&gVal);

        if (!gStr) {
                return false;
        }

        bool found = (std::string (gStr).find (impl->query) != std::string::npos);
        BOOST_LOG (lg) << "---- [" << impl->query << "] =? [" << gStr << "], found = " << found;
        return found;
}

} // namespace
