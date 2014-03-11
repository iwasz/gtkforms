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

namespace GtkForms {
static src::logger_mt& lg = logger::get();

void Mapping::view2Model (MappingDTO *dto)
{
        view2Model (dto, input, property, model);
}

/*--------------------------------------------------------------------------*/

void Mapping::model2View (MappingDTO *dto)
{
        model2View (dto, input, property, model);
}

/*--------------------------------------------------------------------------*/

void Mapping::view2Model (MappingDTO *dto, std::string const &input, std::string const &property, std::string const &model)
{
        Wrapper::BeanWrapper *wrapper = dto->app->getBeanWrapper ();

        std::string finalProperty;
        std::string finalModelName;

        if (!property.empty ()) {
                finalProperty = property;
        }
        else {
                finalProperty = dto->app->getDefaultProperty ("");
        }

        if (!model.empty ()) {
                finalModelName = model;
        }
        else {
                finalModelName = input;
        }

        // TODO, to by można było bez Wrappera zrobić, tylko na piechotę. Szybciej by było.
        wrapper->setWrappedObject (Core::Variant (dto->inputWidget));
        Core::Variant v = wrapper->get (finalProperty);

        if (v.isNone ()) {
                throw Core::Exception ("Mapping::view2Model. Invalid property : [" + finalProperty + "] in input widget : [" + input + "].");
        }

        BOOST_LOG (lg) << "Maping::view->model : " << input << "." << finalProperty << "(" << v << ")" << " -> " << finalModelName;

        Core::VariantMap &unitScope = dto->context->getUnitScope ();
        wrapper->setWrappedObject (Core::Variant (&unitScope));
        wrapper->set (finalModelName, v);
}

/*--------------------------------------------------------------------------*/

void Mapping::model2View (MappingDTO *dto, std::string const &input, std::string const &property, std::string const &model)
{
        Wrapper::BeanWrapper *wrapper = dto->app->getBeanWrapper ();

        std::string finalProperty;
        std::string finalModelName;

        if (!property.empty ()) {
                finalProperty = property;
        }
        else {
                finalProperty = dto->app->getDefaultProperty ("");
        }

        if (!model.empty ()) {
                finalModelName = model;
        }
        else {
                finalModelName = input;
        }

        Core::VariantMap &unitScope = dto->context->getUnitScope ();
        wrapper->setWrappedObject (Core::Variant (&unitScope));
        Core::Variant v = wrapper->get (finalModelName);

        if (v.isNone ()) {
                return;
//                throw Core::Exception ("Mapping::model2View. Invalid modelName : [" + finalModelName + "]. Property you've tried to set : [" + finalProperty + "] in input widget : [" + input + "].");
        }

        BOOST_LOG (lg) << "Mapping::model->view : " << finalModelName << "(" << v << ")" << " -> " << input << "." << finalProperty;

        wrapper->setWrappedObject (Core::Variant (dto->inputWidget));
        wrapper->set (finalProperty, v);
}

} /* namespace GtkForms */
