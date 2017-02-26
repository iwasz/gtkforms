/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef IMAPPING_H_
#define IMAPPING_H_

#include "MappingDTO.h"
#include "ValidationAndBindingResult.h"
#include <ReflectionParserAnnotation.h>
#include <core/Object.h>
#include <gtk/gtk.h>
#include <string>

namespace GtkForms {
class App;
class Context;

class __tiliae_reflect__ IMapping : public Core::Object {
public:
        virtual ~IMapping () {}

        /**
         * Name of input widget on the UI.
         */
        virtual std::string getWidget () const = 0;

        /**
         * Name of model property which is mapped to the view.
         */
        virtual std::string getModel () const = 0;

        /**
         * Name of the widget property which we are seting when converting from model
         * to view and vice versa.
         */
        virtual std::string getProperty () const = 0;

        /**
         * Instead of setting the model name, you can set the constValue property. This way
         * the mapping will not look for model, but will always return the constValue.
         */
        virtual Core::Variant getConstValue () const = 0;

        virtual ValidationAndBindingResult view2Model (MappingDTO *dto) __tiliae_no_reflect__ = 0;
        virtual ValidationAndBindingResult view2Model (MappingDTO *dto, std::string const &widgetName, std::string const &propertyName,
                                                       std::string const &modelName, Editor::IEditor *editor = nullptr) __tiliae_no_reflect__ = 0;

        virtual void model2View (MappingDTO *dto) __tiliae_no_reflect__ = 0;
        virtual void model2View (MappingDTO *dto, std::string const &widgetName, std::string const &propertyName, std::string const &modelName,
                                 Editor::IEditor *editor = nullptr) __tiliae_no_reflect__ = 0;
};

} /* namespace GtkForms */

#endif /* IMAPPING_H_ */
