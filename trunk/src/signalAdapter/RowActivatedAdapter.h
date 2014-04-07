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
 * Parameters :
 * 0 : GtkTreeView
 * 1 : GtkTreePath (in string form).
 * 2 : index of column cicked (starting from 0).
 * 3 : whole model.
 */
class RowActivatedAdapter : public AbstractSignalAdapter {
public:
        ctr__ (void)
        bse_ ("AbstractSignalAdapter")

        RowActivatedAdapter () : modelColumn (NO_MODEL_COLUMN) {}
        virtual ~RowActivatedAdapter () {}
        std::string getSignal () const { return "row-activated"; }

        Core::VariantVector adapt (guint n_param_values, const GValue *param_values) const;

        const int NO_MODEL_COLUMN = -2;

private:

        int prp_ (modelColumn);
        end_ (RowActivatedAdapter)
};

} /* namespace GtkForms */
#endif /* ROWACTIVATEDADAPTER_H_ */
