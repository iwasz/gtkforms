/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ROWACTIVATEDADAPTER_H_
#define ROWACTIVATEDADAPTER_H_

#include "AbstractSignalAdapter.h"

namespace GtkForms {

/**
 * row-activated Parameters :
 * 0 : whole model.
 * 1 : GtkTreeView
 * 2 : GtkTreePath (in string form).
 * 3 : index of column cicked (starting from 0).
 *
 * button-press-event / button-release-event
 * 0 : whole model
 * 1 : GtkWidget *widget,
 *
 * changed
 * 0 : whole model
 * 1 : GtkTreeSelection *selection,
 */
class __tiliae_reflect__ RowActivatedAdapter : public AbstractSignalAdapter {
public:
        RowActivatedAdapter () : modelColumn (NO_MODEL_COLUMN) {}
        virtual ~RowActivatedAdapter () {}

        std::string getSignal () const { return signal; }
        void setSignal (std::string const &s) { signal = s; }

        Core::VariantVector adapt (guint n_param_values, const GValue *param_values) const;

        int getModelColumn () const { return modelColumn; }
        void setModelColumn (int value) { modelColumn = value; }

private:
        Core::VariantVector adaptRowActivated (guint n_param_values, const GValue *param_values) const;
        Core::VariantVector adaptButtonPressEvent (guint n_param_values, const GValue *param_values) const;
        Core::VariantVector adaptSelectionChanged (guint n_param_values, const GValue *param_values) const;

        const int NO_MODEL_COLUMN = -2;
        int modelColumn;
        std::string signal = "row-activated";
};

} /* namespace GtkForms */
#endif /* ROWACTIVATEDADAPTER_H_ */
