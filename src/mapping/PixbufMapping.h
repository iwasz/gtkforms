/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef RAWTOPIXBUF_H_
#define RAWTOPIXBUF_H_

#include "AssociationMap.h"
#include "Mapping.h"
#include <GtkForms.h>
#include <ReflectionParserAnnotation.h>
#include <vector>

namespace GtkForms {

/**
 *
 */
class __tiliae_reflect__ PixbufMapping : public Mapping {
public:
        PixbufMapping () : maxWidth{ -1 }, maxHeight{ -1 } {}
        virtual ~PixbufMapping () {}

        virtual ValidationAndBindingResult view2Model (MappingDTO *dto, std::string const &widgetName, std::string const &propertyName,
                                                       std::string const &modelName, Editor::IEditor *editor = nullptr) __tiliae_no_reflect__
        {
                return ValidationAndBindingResult{};
        }

protected:
        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

public:
        int maxWidth;
        int maxHeight;
        AssociationMap dict;
};

} // end namespace

#endif /* RAWTOPIXBUF_H_ */
