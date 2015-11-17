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
#include <ReflectionParserAnnotation.h>

namespace GtkForms {

/**
 *
 */
class __tiliae_reflect__ PixbufColumn : public Column {
public:
        virtual ~PixbufColumn () {}
        virtual ValidationAndBindingResult view2Model (MappingDTO *dto) { return ValidationAndBindingResult {}; /* Not implemented and won't be */ }

protected:

        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

public:

        AssociationMap dict;

};

} // end namespace

#endif /* RAWTOPIXBUF_H_ */
