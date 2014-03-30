/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_TABLE_MAPPING_COLUMN_H_
#define GTK_FORMS_TABLE_MAPPING_COLUMN_H_

#include <Tiliae.h>
#include "ReflectionMacros.h"

namespace GtkForms {

struct Column {
        ctr__ (void)
        Column () : m2vEditor {nullptr}, v2mEditor {nullptr} {}
        std::string prp_ (model);
        Editor::IEditor *prp_ (m2vEditor);
        Editor::IEditor *prp_ (v2mEditor);
        end_ (Column)
};

typedef std::vector <Column *> ColumnVector;
col_ (ColumnVector)

} /* namespace GtkForms */
#endif /* COLUMN_H_ */
