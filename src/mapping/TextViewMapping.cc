/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "TextViewMapping.h"
#include "Logging.h"
#include "App.h"
#include "Context.h"
#include <gtk/gtk.h>
#include <Tiliae.h>
#include "GValueVariant.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get();
using namespace Core;

void TextViewMapping::view2Model (MappingDTO *dto)
{
//        GtkAdjustment *adjustment = 0;
//        std::string finalProperty;
//        std::string finalModelName;
//
//        if (GTK_IS_RANGE (dto->inputWidget)) {
//                adjustment = gtk_range_get_adjustment (GTK_RANGE (dto->inputWidget));
//        }
//        else if (GTK_IS_SPIN_BUTTON (dto->inputWidget)) {
//                adjustment = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (dto->inputWidget));
//        }
//        else {
//                throw Core::Exception ("TextViewMapping::view2Model : widget type not supported. Supported types : GtkSpinButton, GtkRange.");
//        }
//
//        if (!adjustment) {
//                throw Core::Exception ("TextViewMapping::view2Model : adjustment is NULL.");
//        }
//
//        if (!property.empty ()) {
//                finalProperty = property;
//        }
//        else {
//                finalProperty = "value";
//        }
//
//        if (!model.empty ()) {
//                finalModelName = model;
//        }
//        else {
//                finalModelName = input;
//        }
//
//        GParamSpec *spec = g_object_class_find_property (G_OBJECT_GET_CLASS (adjustment), finalProperty.c_str ());
//
//        if (!spec) {
//                throw Core::Exception ("TextViewMapping::view2Model : Non-existent GtkAdjustment property has been requested. Property name : [" + finalProperty + "]");
//        }
//
//        GType propType = spec->value_type;
//        GValue propValue = {0};
//
//        g_value_init (&propValue, propType);
//        g_object_get_property (G_OBJECT (adjustment), finalProperty.c_str (), &propValue);
//        Core::Variant v = gValueToVariant (&propValue);
//        g_value_unset (&propValue);
//
//        if (v.isNone ()) {
//                throw Core::Exception ("TextViewMapping::view2Model. Invalid property : [" + finalProperty + "] in input widget : [" + input + "].");
//        }
//
//        BOOST_LOG (lg) << "Maping::view->model : " << input << "." << finalProperty << "(" << v << ")" << " -> " << finalModelName;
//
//        Wrapper::BeanWrapper *wrapper = dto->app->getBeanWrapper ();
//        Core::VariantMap &unitScope = dto->context->getUnitScope ();
//        wrapper->setWrappedObject (Core::Variant (&unitScope));
//
//        Core::DebugContext ctx;
//        if (!wrapper->set (finalModelName, v, &ctx)) {
//                Core::Exception e {"Mapping::view2Model : invalid attempt to assign a value to a model property. "};
//                e.addContext (ctx);
//                throw e;
//        }
//
}

/*--------------------------------------------------------------------------*/

void TextViewMapping::model2View (MappingDTO *dto)
{
        GtkTextBuffer *buffer = 0;
        std::string finalProperty;
        std::string finalModelName;

        if (GTK_TEXT_VIEW (dto->inputWidget)) {
                buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (dto->inputWidget));
        }
        else {
                throw Core::Exception ("TextViewMapping::view2Model : widget type not supported. Only GtkTextView is supported.");
        }

        if (!buffer) {
                throw Core::Exception ("TextViewMapping::view2Model : buffer inside GtkTextView is NULL.");
        }

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

        Wrapper::BeanWrapper *wrapper = dto->app->getBeanWrapper ();
        wrapper->setWrappedObject (Core::Variant (dto->context));
        Core::Variant v = wrapper->get (finalModelName);

        if (v.isNone ()) {
                return;
        }

        GValue gVal = G_VALUE_INIT;
        variantToGValue (&gVal, v);
        g_object_set_property (G_OBJECT (buffer), finalProperty.c_str (), &gVal);
        g_value_unset (&gVal);
}

} /* namespace GtkForms */
