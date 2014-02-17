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

class TableMapping : public IMapping {
public:
        ctr__ (void)
        virtual ~TableMapping () {}

        std::string getInput () const { return listStore; }

        void view2Model (MappingDTO *dto);
        void model2View (MappingDTO *dto);

private:

        std::string prp_ (listStore);
        std::string prp_ (modelCollection);
        ColumnVector prr_ (columns);

        end_ (TableMapping)
};

} /* namespace GtkForms */
#endif /* TABLEMAPPING_H_ */
