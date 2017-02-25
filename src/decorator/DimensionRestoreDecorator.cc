/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "DimensionRestoreDecorator.h"
#include "Logging.h"
#include "view/AbstractView.h"

namespace GtkForms {
static src::logger_mt &lg = logger::get ();

void DimensionRestoreDecorator::preShow (AbstractView *view, Context *ctx)
{
        if (!database) {
                BOOST_LOG (lg) << "DimensionRestoreDecorator::preShow : no database.";
                return;
        }

        GObject *wid = view->getUiOrThrow (widget);

        if (GTK_IS_WINDOW (wid)) {
                GtkWindow *win = GTK_WINDOW (wid);
                g_signal_connect (win, "size-allocate", G_CALLBACK (&DimensionRestoreDecorator::onSizeAllocate), this);
                DimensionRestoreDatabase::ValueVector values = database->get (widget, key);

                if (values.size () < 2) {
                        return;
                }

                gtk_window_resize (win, values[0], values[1]);
        }
        else if (GTK_IS_PANED (wid)) {
                GtkPaned *paned = GTK_PANED (wid);
                g_signal_connect (G_OBJECT (paned), "notify::position", G_CALLBACK (&DimensionRestoreDecorator::onPanedPositionNotify), this);

                DimensionRestoreDatabase::ValueVector values = database->get (widget, key);

                if (values.size () < 1) {
                        return;
                }

                gtk_paned_set_position (paned, values[0]);
        }
}

/*****************************************************************************/

void DimensionRestoreDecorator::onSizeAllocate (GtkWidget *widget, GdkRectangle *allocation, gpointer userData)
{
        DimensionRestoreDecorator *decorator = static_cast<DimensionRestoreDecorator *> (userData);
        DimensionRestoreDatabase *database = decorator->database;

        DimensionRestoreDatabase::ValueVector values;
        values.push_back (allocation->width);
        values.push_back (allocation->height);
        database->set (decorator->widget, decorator->key, values);
}

/*****************************************************************************/

void DimensionRestoreDecorator::onPanedPositionNotify (GObject *gobject, GParamSpec *pspec, gpointer userData)
{
        GType propType = pspec->value_type;
        GValue propValue = { 0 };

        g_value_init (&propValue, propType);
        g_object_get_property (gobject, "position", &propValue);
        int pos = g_value_get_int (&propValue);
        g_value_unset (&propValue);

        DimensionRestoreDecorator *decorator = static_cast<DimensionRestoreDecorator *> (userData);
        DimensionRestoreDatabase *database = decorator->database;

        DimensionRestoreDatabase::ValueVector values;
        values.push_back (pos);
        database->set (decorator->widget, decorator->key, values);
}

/*****************************************************************************/

void DimensionRestoreDatabase::set (std::string const &widgetName, std::string const &additionalKey, ValueVector const &v)
{
        values[widgetName + additionalKey] = v;
}

/*****************************************************************************/

DimensionRestoreDatabase::ValueVector DimensionRestoreDatabase::get (std::string const &widgetName, std::string const &additionalKey) const
{
        if (values.find (widgetName + additionalKey) != values.cend ()) {
                return values.at (widgetName + additionalKey);
        }

        return ValueVector ();
}

} // namespace
