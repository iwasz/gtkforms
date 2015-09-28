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
#include <ReflectionParserAnnotation.h>
#include "MappingDTO.h"

namespace GtkForms {

struct ColumnElementDTO : public ViewElementDTO {
        ColumnElementDTO () : ViewElementDTO {nullptr} {}
        GtkTreeIter *iter = nullptr;
        unsigned int columnNumber = 0;
};

struct __tiliae_reflect__ Column : public Mapping {
        REFLECTION_CONSTRUCTOR_ (void)
        REFLECTION_BASE_CLASS ("Mapping")
        virtual ~Column () {}

protected:

        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

        REFLECTION_END (Column)
};

typedef std::vector <Column *> ColumnVector;
REFLECTION_COLLECTION (ColumnVector)

} /* namespace GtkForms */
#endif /* COLUMN_H_ */
