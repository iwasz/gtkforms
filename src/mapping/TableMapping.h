/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef TABLEMAPPING_H_
#define TABLEMAPPING_H_

#include "IMapping.h"
#include <ReflectionParserAnnotation.h>
#include "Column.h"

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

        std::string getWidget () const { return input; }
        std::string getModel () const { return model; }

        ValidationAndBindingResult view2Model (MappingDTO *dto);
        void model2View (MappingDTO *dto);

public:

        std::string input;
        std::string model;
        ColumnVector columns;
        Core::VariantVector modelColumnCopy;
};

} /* namespace GtkForms */
#endif /* TABLEMAPPING_H_ */
