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
 * Maps model data onto GtkImage widgets. It can perform in different ways:
 * - If model data (valueToSet parameter in setToView method) is of type RawData,
 *   it is converted to GdkPixbuf and displayed.
 * - If dict has at leas 1 element, model data (valueToSet parameter in setToView method)
 *   will be converterd to string, and used as a key to obtain value from dict. Dict
 *   values would be then interpreted as path to image files, which would the be displayed.
 * - Id dict is empty, model data (valueToSet parameter in setToView method) would be
 *   casted to string, and interpreted as path to image file.
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
