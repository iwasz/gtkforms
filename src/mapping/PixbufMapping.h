/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef RAWTOPIXBUF_H_
#define RAWTOPIXBUF_H_

#include <GtkForms.h>
#include "Mapping.h"
#include <vector>
#include "AssociationMap.h"
#include <ReflectionParserAnnotation.h>

namespace GtkForms {

/**
 *
 */
class __tiliae_reflect__ PixbufMapping : public Mapping {
public:
        PixbufMapping () : maxWidth {-1}, maxHeight {-1} {}
        virtual ~PixbufMapping () {}

        virtual ValidationAndBindingResult view2Model (MappingDTO *dto) { /* Not implemented and won't be */ return ValidationAndBindingResult{}; }

protected:

        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

public:

        int maxWidth;
        int maxHeight;
        AssociationMap dict;
};

} // end namespace

#endif /* RAWTOPIXBUF_H_ */
