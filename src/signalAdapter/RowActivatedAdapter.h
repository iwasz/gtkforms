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
 */
class RowActivatedAdapter : public AbstractSignalAdapter {
public:
        ctr__ (void)
        bse_ ("AbstractSignalAdapter")

        virtual ~RowActivatedAdapter () {}
        std::string getSignal () const { return "row-activated"; }

        Core::VariantVector adapt (guint n_param_values, const GValue *param_values) const;

private:

        end_ (RowActivatedAdapter) // __LINE__, __COUNTER__ conflict!
};

} /* namespace GtkForms */
#endif /* ROWACTIVATEDADAPTER_H_ */
