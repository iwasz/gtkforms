/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ADJUSTMENT_TABLEMAPPING_H_
#define ADJUSTMENT_TABLEMAPPING_H_

#include "Mapping.h"
#include "ReflectionMacros.h"
#include "Column.h"

namespace GtkForms {

/**
 * IMapping which converts data between the model and the view designed to be used with GtkAdjustment.
 * First thing this mapper tries is to cast MappingDTO::inputWidget (widget from the value is to be retreived)
 * to some concrete type which uses GtkAdjustment (such as GtkScale or GtkSpinButton).
 */
class AdjustmentMapping : public Mapping {
public:
        REFLECTION_CONSTRUCTOR_ (void)
        REFLECTION_BASE_CLASS ("Mapping")
        virtual ~AdjustmentMapping () {}

protected:

        virtual std::string getDefaultProperty (App *app, std::string const &widgetClass) const { return "value"; }
        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);
        virtual Core::Variant getFromView (ViewElementDTO *viewObject, std::string const &finalProperty);

        REFLECTION_END (AdjustmentMapping)
};

} /* namespace GtkForms */

#endif /* TABLEMAPPING_H_ */
