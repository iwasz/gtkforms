/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef TABLEMAPPING_H_
#define TABLEMAPPING_H_

#include "Column.h"
#include "IMapping.h"
#include <ReflectionParserAnnotation.h>

namespace GtkForms {

/**
 * IMapping which converts data between the model and the view designed to be used with tables. The model is
 * a collection which can be accessible by Tiliae.Reflection (like std::list, std::vector etc).
 * This collection may store complicated entities (I mean objects of some classes which are exposed
 * into Tiliae.Reflection as well). At the other hand, the view is a GtkTreeView widget with GtkListStore
 * attached to it. This TableMapping class gets this GtkListStore from GtkTreeView, and upon information
 * about columns it tries to put model data into that listStore.
 *
 * If you plan to have more than one GtkTreeView pointing to the same GtkListStore, make only one of
 * those GtkTreeViews an "input". In other words, only one of GtkTreeViews having common GtkListStore
 * should have name beginnig with "!". This is done that way for consistency, because only *widgets* are
 * inputs even though we are changing GtkListStore here (which is another form of model).
 */
class __tiliae_reflect__ TableMapping : public IMapping {
public:
        virtual ~TableMapping () {}

        std::string getWidget () const { return widget; }
        std::string getModel () const { return model; }

        ValidationAndBindingResult view2Model (MappingDTO *dto) __tiliae_no_reflect__ { return view2Model (dto, widget, "", model, nullptr); }
        virtual ValidationAndBindingResult view2Model (MappingDTO *dto, std::string const &widgetName, std::string const &propertyName,
                                                       std::string const &modelName, Editor::IEditor *editor = nullptr) __tiliae_no_reflect__;

        void model2View (MappingDTO *dto) __tiliae_no_reflect__ { model2View (dto, widget, "", model, nullptr); }
        virtual void model2View (MappingDTO *dto, std::string const &widgetName, std::string const &propertyName, std::string const &modelName,
                                 Editor::IEditor *editor = nullptr) __tiliae_no_reflect__;

public:
        std::string widget;
        std::string model;
        ColumnVector columns;
        Core::VariantVector modelColumnCopy;
};

} /* namespace GtkForms */
#endif /* TABLEMAPPING_H_ */
