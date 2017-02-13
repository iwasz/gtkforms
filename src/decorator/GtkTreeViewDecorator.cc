/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "GtkTreeViewDecorator.h"
#include "view/AbstractView.h"

namespace GtkForms {

void GtkTreeViewDecorator::expandAll (AbstractView *view)
{
        GObject *widget = view->getUiOrThrow (getWidget ());

        if (!GTK_IS_TREE_VIEW (widget)) {
                throw Core::Exception ("GtkTreeViewDecorator::postStart : widget [" + getWidget () + "] is not a GtkTreeView");
        }

        GtkTreeView *treeView = GTK_TREE_VIEW (widget);
        gtk_tree_view_expand_all (treeView);
}

/*****************************************************************************/

void GtkTreeViewDecorator::postStart (AbstractView *view, Context *)
{
        if (postStartExpandAll) {
                expandAll (view);
        }
}

/*****************************************************************************/

void GtkTreeViewDecorator::postRefresh (AbstractView *view, Context *)
{
        if (postRefreshExpandAll) {
                expandAll (view);
        }
}

} // GtkForms
