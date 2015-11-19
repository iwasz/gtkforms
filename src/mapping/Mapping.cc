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
#include <gtk/gtk.h>

namespace GtkForms {
static src::logger_mt& lg = logger::get();

ValidationAndBindingResult Mapping::view2Model (MappingDTO *dto)
{
        std::string finalProperty;
        std::string finalModelName;
        finalPropertyAndModel (&finalProperty, &finalModelName, dto);

        Core::Variant v = getFromView (dto->viewElement, finalProperty);

        if (v.isNone ()) {
                throw Core::Exception ("Mapping::view2Model. Invalid property : [" + finalProperty + "] in input widget : [" + widget + "].");
        }

#if 0
        BOOST_LOG (lg) << "Maping::view->model : " << input << "." << finalProperty << "(" << v << ")" << " -> " << finalModelName;
#endif
        return setToModel (dto->app->getBeanWrapper (), dto->v2mModelObject, finalModelName, v);
}

/*--------------------------------------------------------------------------*/

void Mapping::model2View (MappingDTO *dto)
{
        std::string finalProperty;
        std::string finalModelName;
        finalPropertyAndModel (&finalProperty, &finalModelName, dto);

        Core::Variant v = getFromModel (dto->app->getBeanWrapper (), dto->m2vModelObject, finalModelName);

        if (v.isNone ()) {
                if (dto->app->getConfig ()->logMappings) {
                        BOOST_LOG (lg) << "Mapping::model2View model \033[31m[" << finalModelName << "]\033[0m is empty (isNone () == true).";
                }

                return;
        }

        if (dto->app->getConfig ()->logMappings) {
                BOOST_LOG (lg) << "Mapping::model2View : " << finalModelName << "(" << v << ")" << " -> " << widget << "." << finalProperty;
        }

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
                *finalProperty = getDefaultProperty (dto->app, dto->viewElement);
        }

        if (!model.empty ()) {
                *finalModelName = model;
        }
        else {
                *finalModelName = widget;
        }
}

/*--------------------------------------------------------------------------*/

std::string Mapping::getDefaultProperty (App *app, ViewElementDTO *dto) const
{
        GObject *inputWidget = dto->inputWidget;

        // UWAGA! Od szczegółu do ogółu!
        if (GTK_IS_SWITCH (inputWidget) || GTK_IS_TOGGLE_BUTTON (inputWidget)) {
                return "active";
        }
        else if (GTK_IS_LABEL (inputWidget) || GTK_IS_BUTTON (inputWidget)) {
                return "label";
        }

        if (app->getConfig ()->logMappings) {
                BOOST_LOG (lg) << "Mapping::getDefaultProperty : no default property found. Returning 'text'.";
        }

        return "text";
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

ValidationAndBindingResult Mapping::setToModel (Wrapper::BeanWrapper *wrapper, Core::Variant objectToWrap, std::string const &finalModelName, Core::Variant valueToSet)
{
        ValidationAndBindingResult result;
        result.model = finalModelName;
        wrapper->setWrappedObject (objectToWrap);

        Core::Variant output;
        if (v2mEditor) {
                Core::DebugContext ctx;
                if (!v2mEditor->convert (valueToSet, &output, &ctx)) {
                        result.valid = false;
                        result.params["message"] = Core::Variant {"Conversion failed. Input value : " + valueToSet.toString ()};
                        ctx.addMessage ("Mapping::view2Model : Nie udało się dokonać konwersji. Wartość wejściowa to : " + valueToSet.toString ());
                        BOOST_LOG (lg) << ctx.getMessage ();
                        return result;
                }
        }
        else {
                output = valueToSet;
        }

        Core::DebugContext ctx;
        if (!wrapper->set (finalModelName, valueToSet, &ctx)) {
                result.valid = false;
                result.params["message"] = Core::Variant {"Invalid attempt to assign a value to a model property."};
                ctx.addMessage ("Mapping::view2Model : invalid attempt to assign a value to a model property. ");
                BOOST_LOG (lg) << ctx.getMessage ();
                return result;
        }

        return result;
}

/*--------------------------------------------------------------------------*/

ValidationAndBindingResult Mapping::view2Model (MappingDTO *dto, std::string const &input, std::string const &property, std::string const &model, Editor::IEditor *editor)
{
        static Mapping mapping;
        mapping.widget = input;
        mapping.property = property;
        mapping.model = model;
        mapping.v2mEditor = editor;
        mapping.m2vEditor = nullptr;
        return mapping.view2Model (dto);
}

/*--------------------------------------------------------------------------*/

void Mapping::model2View (MappingDTO *dto, std::string const &input, std::string const &property, std::string const &model, Editor::IEditor *editor)
{
        static Mapping mapping;
        mapping.widget = input;
        mapping.property = property;
        mapping.model = model;
        mapping.v2mEditor = nullptr;
        mapping.m2vEditor = editor;
        mapping.model2View (dto);
}

} /* namespace GtkForms */
