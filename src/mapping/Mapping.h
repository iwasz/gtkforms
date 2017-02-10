/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef GTK_FORMS_MAPPING_H_
#define GTK_FORMS_MAPPING_H_

#include "IMapping.h"
#include "RawData.h"
#include <ReflectionParserAnnotation.h>
#include <Tiliae.h>
#include <unordered_map>
#include <unordered_set>

namespace GtkForms {
class App;

class __tiliae_reflect__ Mapping : public IMapping {
public:
        Mapping () : m2vEditor{ nullptr }, v2mEditor{ nullptr } {}
        virtual ~Mapping () {}

        virtual std::string getWidget () const { return widget; }
        virtual std::string getProperty () const { return property; }
        virtual std::string getModel () const { return model; }

        ValidationAndBindingResult view2Model (MappingDTO *dto) __tiliae_no_reflect__ { return view2Model (dto, widget, property, model, v2mEditor); }
        virtual ValidationAndBindingResult view2Model (MappingDTO *dto, std::string const &widgetName, std::string const &propertyName,
                                                       std::string const &modelName, Editor::IEditor *editor = nullptr) __tiliae_no_reflect__;

        void model2View (MappingDTO *dto) __tiliae_no_reflect__ { model2View (dto, widget, property, model, m2vEditor); }
        virtual void model2View (MappingDTO *dto, std::string const &widgetName, std::string const &propertyName, std::string const &modelName,
                                 Editor::IEditor *editor = nullptr) __tiliae_no_reflect__;

        virtual Core::Variant getConstValue () const { return constValue; }

protected:
        virtual void finalPropertyAndModel (std::string *finalProperty, std::string *finalModelName, MappingDTO *dto, const std::__cxx11::string &widgetName,
                                            const std::__cxx11::string &propertyName, const std::__cxx11::string &modelName);

        virtual Core::Variant getFromModel (Wrapper::BeanWrapper *wrapper, Core::Variant objectToWrap, std::string const &finalModelName,
                                            Editor::IEditor *m2vEditor);
        virtual ValidationAndBindingResult setToModel (Wrapper::BeanWrapper *wrapper, Core::Variant objectToWrap, std::string const &finalModelName,
                                                       Core::Variant valueToSet, Editor::IEditor *view2modelEditor);
        virtual Core::Variant getFromView (ViewElementDTO *viewObject, std::string const &finalProperty);
        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

        virtual std::string getDefaultProperty (App *app, ViewElementDTO *dto) const;

public:
        std::string widget;
        std::string property;
        std::string model;
        Editor::IEditor *m2vEditor;
        Editor::IEditor *v2mEditor;
        Core::Variant constValue;
};

typedef __tiliae_reflect__ std::vector<IMapping *> MappingVector;

typedef std::unordered_map<std::string, IMapping *> MappingMap;
typedef std::unordered_multimap<std::string, IMapping *> MappingMultiMap;

} /* namespace GtkForms */

#endif /* MAPPING_H_ */
