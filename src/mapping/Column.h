/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_TABLE_MAPPING_COLUMN_H_
#define GTK_FORMS_TABLE_MAPPING_COLUMN_H_

#include "Mapping.h"
#include "MappingDTO.h"
#include <ReflectionParserAnnotation.h>
#include <Tiliae.h>

namespace GtkForms {

struct ColumnElementDTO : public ViewElementDTO {
        ColumnElementDTO () : ViewElementDTO{ nullptr } {}
        virtual ~ColumnElementDTO () {}

        GtkListStore *listStore = nullptr;
        GtkTreeStore *treeStore = nullptr;
        GtkTreeIter *iter = nullptr;
        unsigned int columnNumber = 0;
};

struct __tiliae_reflect__ Column : public Mapping {
        virtual ~Column () {}

protected:
        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);
};

// typedef __tiliae_reflect__ std::vector <Column *> ColumnVector;

} /* namespace GtkForms */
#endif /* COLUMN_H_ */
