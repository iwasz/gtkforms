/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef TEXT_VIEW_MAPPING_H_
#define TEXT_VIEW_MAPPING_H_

#include <ReflectionParserAnnotation.h>
#include "Column.h"
#include "Mapping.h"

namespace GtkForms {

class __tiliae_reflect__ TextViewMapping : public Mapping {
public:
        virtual ~TextViewMapping () {}

protected:

        virtual std::string getDefaultProperty (App *app, std::string const &widgetClass) const { return "text"; }
        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);
        virtual Core::Variant getFromView (ViewElementDTO *viewObject, std::string const &finalProperty);
};

} /* namespace GtkForms */
#endif /* TABLEMAPPING_H_ */
