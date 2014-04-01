/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "AdjustmentMapping.h"
#include "Logging.h"
#include "App.h"
#include "Context.h"
#include <gtk/gtk.h>
#include <Tiliae.h>
#include "GValueVariant.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get();
using namespace Core;

void AdjustmentMapping::view2Model (MappingDTO *dto)
{
        GtkAdjustment *adjustment = 0;

        if (GTK_IS_RANGE (dto->inputWidget)) {
                adjustment = gtk_range_get_adjustment (GTK_RANGE (dto->inputWidget));
        }
        else if (GTK_IS_SPIN_BUTTON (dto->inputWidget)) {
                adjustment = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (dto->inputWidget));
        }
        else {
                throw Core::Exception ("AdjustmentMapping::view2Model : widget type not supported. Supported types : GtkSpinButton, GtkRange.");
        }

        if (!adjustment) {
                throw Core::Exception ("AdjustmentMapping::view2Model : adjustment is NULL.");
        }

        std::string finalProperty;
        std::string finalModelName;

        if (!property.empty ()) {
                finalProperty = property;
        }
        else {
                finalProperty = "value";
        }

        if (!model.empty ()) {
                finalModelName = model;
        }
        else {
                finalModelName = input;
        }

        GParamSpec *spec = g_object_class_find_property (G_OBJECT_GET_CLASS (adjustment), finalProperty.c_str ());

        if (!spec) {
                throw Core::Exception ("AdjustmentMapping::view2Model : Non-existent GtkAdjustment property has been requested. Property name : [" + finalProperty + "]");
        }

        GType propType = spec->value_type;
        GValue propValue = {0};

        g_value_init (&propValue, propType);
        g_object_get_property (G_OBJECT (adjustment), finalProperty.c_str (), &propValue);
        Core::Variant v = gValueToVariant (&propValue);
        g_value_unset (&propValue);

        if (v.isNone ()) {
                throw Core::Exception ("AdjustmentMapping::view2Model. Invalid property : [" + finalProperty + "] in input widget : [" + input + "].");
        }

        BOOST_LOG (lg) << "Maping::view->model : " << input << "." << finalProperty << "(" << v << ")" << " -> " << finalModelName;

        Wrapper::BeanWrapper *wrapper = dto->app->getBeanWrapper ();
        Core::VariantMap &unitScope = dto->context->getUnitScope ();
        wrapper->setWrappedObject (Core::Variant (&unitScope));

        Core::DebugContext ctx;
        if (!wrapper->set (finalModelName, v, &ctx)) {
                Core::Exception e {"Mapping::view2Model : invalid attempt to assign a value to a model property. "};
                e.addContext (ctx);
                throw e;
        }
}

/*--------------------------------------------------------------------------*/

void AdjustmentMapping::model2View (MappingDTO *dto)
{
}

} /* namespace GtkForms */
