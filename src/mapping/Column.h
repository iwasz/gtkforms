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
#include "Mapping.h"
#include "ReflectionMacros.h"
#include "MappingDTO.h"

namespace GtkForms {

struct ColumnElementDTO : public ViewElementDTO {
        ColumnElementDTO () : ViewElementDTO {nullptr} {}
        GtkTreeIter *iter = nullptr;
        unsigned int columnNumber = 0;
};

struct Column : public Mapping {
        ctr__ (void)
        bse_ ("Mapping")
        virtual ~Column () {}

protected:

        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

        end_ (Column)
};

typedef std::vector <Column *> ColumnVector;
col_ (ColumnVector)

} /* namespace GtkForms */
#endif /* COLUMN_H_ */