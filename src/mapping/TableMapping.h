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
#include "ReflectionMacros.h"
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
class TableMapping : public IMapping {
public:
        ctr__ (void)
        virtual ~TableMapping () {}

        std::string getInput () const { return input; }

        void view2Model (MappingDTO *dto);
        void model2View (MappingDTO *dto);

private:

        std::string prp_ (input);
        std::string prp_ (modelCollection);
        ColumnVector prr_ (columns);

        end_ (TableMapping)
};

} /* namespace GtkForms */
#endif /* TABLEMAPPING_H_ */
