/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Mapping.h"
#include "Logging.h"
#include "App.h"
#include "Context.h"
#include "GValueVariant.h"
#include <cassert>

namespace GtkForms {
static src::logger_mt& lg = logger::get();

void Mapping::view2Model (MappingDTO *dto)
{
        std::string finalProperty;
        std::string finalModelName;
        finalPropertyAndModel (&finalProperty, &finalModelName, dto);

        Core::Variant v = getFromView (dto->viewElement, finalProperty);

        if (v.isNone ()) {
                throw Core::Exception ("Mapping::view2Model. Invalid property : [" + finalProperty + "] in input widget : [" + input + "].");
        }

        BOOST_LOG (lg) << "Maping::view->model : " << input << "." << finalProperty << "(" << v << ")" << " -> " << finalModelName;
        setToModel (dto->app->getBeanWrapper (), dto->v2mModelObject, finalModelName, v);
}

/*--------------------------------------------------------------------------*/

void Mapping::model2View (MappingDTO *dto)
{
        std::string finalProperty;
        std::string finalModelName;
        finalPropertyAndModel (&finalProperty, &finalModelName, dto);

        Core::Variant v = getFromModel (dto->app->getBeanWrapper (), dto->m2vModelObject, finalModelName);

        if (v.isNone ()) {
                return;
        }

        BOOST_LOG (lg) << "Mapping::model->view : " << finalModelName << "(" << v << ")" << " -> " << input << "." << finalProperty;
        setToView (dto->viewElement, finalProperty, v);
}

/****************************************************************************/

void Mapping::finalPropertyAndModel (std::string *finalProperty, std::string *finalModelName, MappingDTO *dto)
{
        assert (finalProperty);
        assert (finalModelName);

        if (!property.empty ()) {
                *finalProperty = property;
        }
        else {
                *finalProperty = getDefaultProperty (dto->app, "");
        }

        if (!model.empty ()) {
                *finalModelName = model;
        }
        else {
                *finalModelName = input;
        }
}

/*--------------------------------------------------------------------------*/

std::string Mapping::getDefaultProperty (App *app, std::string const &widgetClass) const
{
        return app->getDefaultProperty (widgetClass);
}

/*--------------------------------------------------------------------------*/

Core::Variant Mapping::getFromModel (Wrapper::BeanWrapper *wrapper, Core::Variant objectToWrap, std::string const &finalModelName)
{
        wrapper->setWrappedObject (objectToWrap);
        Core::Variant v = wrapper->get (finalModelName);

        if (v.isNone ()) {
                return v;
        }

        Core::Variant output;
        if (m2vEditor) {
                Core::DebugContext ctx;
                if (!m2vEditor->convert (v, &output, &ctx)) {
                        Core::Exception e {"Mapping::getModel : Nie udało się dokonać konwersji. Wartość wejściowa to : " + v.toString ()};
                        e.addContext (ctx);
                        throw e;
                }
        }
        else {
                output = v;
        }

        return output;
}

/*--------------------------------------------------------------------------*/

void Mapping::setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet)
{
        GValue gVal = G_VALUE_INIT;
        variantToGValue (&gVal, valueToSet);
        g_object_set_property (viewObject->inputWidget, finalProperty.c_str (), &gVal);
        g_value_unset (&gVal);
}

/*--------------------------------------------------------------------------*/

Core::Variant Mapping::getFromView (ViewElementDTO *viewObject, std::string const &finalProperty)
{
        GParamSpec *spec = g_object_class_find_property (G_OBJECT_GET_CLASS (viewObject->inputWidget), finalProperty.c_str ());

        if (!spec) {
                throw Core::Exception ("Mapping::view2Model : Non-existent property (of some GTK+ widget) has been requested. Property name : [" + finalProperty + "]");
        }

        GType propType = spec->value_type;
        GValue propValue = {0};

        g_value_init (&propValue, propType);
        g_object_get_property (G_OBJECT (viewObject->inputWidget), finalProperty.c_str (), &propValue);
        Core::Variant v = gValueToVariant (&propValue);
        g_value_unset (&propValue);

        return v;
}

/*--------------------------------------------------------------------------*/

void Mapping::setToModel (Wrapper::BeanWrapper *wrapper, Core::Variant objectToWrap, std::string const &finalModelName, Core::Variant valueToSet)
{
        wrapper->setWrappedObject (objectToWrap);

        Core::Variant output;
        if (v2mEditor) {
                Core::DebugContext ctx;
                if (!v2mEditor->convert (valueToSet, &output, &ctx)) {
                        Core::Exception e {"Mapping::view2Model : Nie udało się dokonać konwersji. Wartość wejściowa to : " + valueToSet.toString ()};
                        e.addContext (ctx);
                        throw e;
                }
        }
        else {
                output = valueToSet;
        }

        Core::DebugContext ctx;
        if (!wrapper->set (finalModelName, valueToSet, &ctx)) {
                Core::Exception e {"Mapping::view2Model : invalid attempt to assign a value to a model property. "};
                e.addContext (ctx);
                throw e;
        }
}

/*--------------------------------------------------------------------------*/

void Mapping::view2Model (MappingDTO *dto, std::string const &input, std::string const &property, std::string const &model, Editor::IEditor *editor)
{
        static Mapping mapping;
        mapping.input = input;
        mapping.property = property;
        mapping.model = model;
        mapping.v2mEditor = editor;
        mapping.m2vEditor = nullptr;
        mapping.view2Model (dto);
}

/*--------------------------------------------------------------------------*/

void Mapping::model2View (MappingDTO *dto, std::string const &input, std::string const &property, std::string const &model, Editor::IEditor *editor)
{
        static Mapping mapping;
        mapping.input = input;
        mapping.property = property;
        mapping.model = model;
        mapping.v2mEditor = nullptr;
        mapping.m2vEditor = editor;
        mapping.model2View (dto);
}

} /* namespace GtkForms */