/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef DIMENSIONRESTOREDECORATOR_H
#define DIMENSIONRESTOREDECORATOR_H

#include "AbstractPageDecorator.h"
#include <gtk/gtk.h>
#include <map>
#include <string>
#include <vector>

namespace GtkForms {

/**
 * Container for storing parameters of widgets.
 */
class __tiliae_reflect__ DimensionRestoreDatabase {
public:
        typedef std::vector<double> ValueVector;

        DimensionRestoreDatabase () { load (); }
        virtual ~DimensionRestoreDatabase () { save (); }

        virtual void load () {}
        virtual void save () {}

        void set (std::string const &widgetName, std::string const &additionalKey, ValueVector const &v);
        ValueVector get (std::string const &widgetName, std::string const &additionalKey) const;

private:
        std::map<std::string, ValueVector> values;
};

/**
 * Stores and restores dimensions of some widgets. Widgets managed:
 * - GtkWindow (size)
 * - GtkPaned
 */
class __tiliae_reflect__ DimensionRestoreDecorator : public AbstractPageDecorator {
public:
        virtual ~DimensionRestoreDecorator () {}

        virtual void preShow (AbstractView *view, Context *ctx);

//        void setDefaultValue (double d)
//        {
//                defaultValueSet = true;
//                defaultValue = d;
//        }

public:
        std::string widget;
        std::string key;
        DimensionRestoreDatabase *database = nullptr;

private:
        static void onSizeAllocate (GtkWidget *widget, GdkRectangle *allocation, gpointer user_data);
        static void onPanedPositionNotify (GObject *gobject, GParamSpec *pspec, gpointer userData);

//        double defaultValue = 0;
//        bool defaultValueSet = false;
};

} // GtkForms

#endif // DIMENSIONRESTOREDECORATOR_H
