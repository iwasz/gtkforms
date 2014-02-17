/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_TABLE_MAPPING_COLUMN_H_
#define GTK_FORMS_TABLE_MAPPING_COLUMN_H_

#include "ReflectionMacros.h"

namespace GtkForms {

struct Column {
        ctr__ (void)
        std::string prp_ (model);
        end_ (Column)
};

typedef std::vector <Column *> ColumnVector;
col_ (ColumnVector)

} /* namespace GtkForms */
#endif /* COLUMN_H_ */
