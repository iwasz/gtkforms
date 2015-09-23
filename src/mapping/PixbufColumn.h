/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef CONSTANTA_RAWTOPIXBUF_H_
#define CONSTANTA_RAWTOPIXBUF_H_

#include <GtkForms.h>
#include "Column.h"
#include "AssociationMap.h"

namespace GtkForms {

/**
 *
 */
class PixbufColumn : public Column {
public:
        REFLECTION_CONSTRUCTOR_ (void)
        REFLECTION_BASE_CLASS ("Column")
        virtual ~PixbufColumn () {}
        virtual ValidationAndBindingResult view2Model (MappingDTO *dto) { return ValidationAndBindingResult {}; /* Not implemented and won't be */ }

protected:

        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

private:

        AssociationMap REFLECTION_FIELD_REFERENCE_INPLACE (dict);

        REFLECTION_END (PixbufColumn)
};

} // end namespace

#endif /* RAWTOPIXBUF_H_ */
