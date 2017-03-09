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
        Core::Variant query;
        int columnNumber = -1;

        static gboolean gtkTreeModelFilterVisibleFunc (GtkTreeModel *model, GtkTreeIter *iter, gpointer data);
};

/*****************************************************************************/

TableFilterMapping::TableFilterMapping () : impl (new Impl) {}

/*****************************************************************************/

TableFilterMapping::~TableFilterMapping () { delete impl; }

/*****************************************************************************/

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

        if (!GTK_IS_TREE_MODEL_FILTER (treeModel)) {
                impl->gtkTreeModelFilter = GTK_TREE_MODEL_FILTER (gtk_tree_model_filter_new (treeModel, nullptr));
                gtk_tree_model_filter_set_visible_func (impl->gtkTreeModelFilter, &TableFilterMapping::Impl::gtkTreeModelFilterVisibleFunc, this, nullptr);
                gtk_tree_view_set_model (treeView, GTK_TREE_MODEL (impl->gtkTreeModelFilter));
        }

        impl->query = /*vcast<std::string> (*/ valueToSet /*)*/;
        BOOST_LOG (lg) << "++++ [" << impl->query << "]";
        gtk_tree_model_filter_refilter (impl->gtkTreeModelFilter);
}

/*****************************************************************************/

int TableFilterMapping::getColumnNumber () const { return impl->columnNumber; }
void TableFilterMapping::setColumnNumber (int value) { impl->columnNumber = value; }

/*****************************************************************************/

gboolean TableFilterMapping::Impl::gtkTreeModelFilterVisibleFunc (GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
        TableFilterMapping *that = static_cast<TableFilterMapping *> (data);

        if (that->impl->query.isNone ()) {
                return true;
        }

        return that->gtkTreeModelFilterVisibleFunc (model, iter, that->impl->query);
}

/*****************************************************************************/

gboolean TableFilterMapping::gtkTreeModelFilterVisibleFunc (GtkTreeModel *model, GtkTreeIter *iter, const Core::Variant &query)
{
        GValue gVal = { 0 };
        gtk_tree_model_get_value (model, iter, getColumnNumber (), &gVal);
        const gchar *gStr = g_value_get_string (&gVal);

        if (!gStr) {
                return false;
        }

        std::string queryStr = vcast<std::string> (query);
        bool found = (std::string (gStr).find (queryStr) != std::string::npos);
#if 0
        BOOST_LOG (lg) << "---- [" << impl->query << "] =? [" << gStr << "], found = " << found;
#endif
        return found;
}

/*****************************************************************************/

// Core::Variant TableFilterMapping::getQuery () const { return impl->query; }

} // namespace
