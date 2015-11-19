/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef GTK_FORMS_MAPPING_H_
#define GTK_FORMS_MAPPING_H_

#include <unordered_map>
#include <unordered_set>
#include "IMapping.h"
#include <ReflectionParserAnnotation.h>
#include <Tiliae.h>
#include "RawData.h"

namespace GtkForms {
class App;

class __tiliae_reflect__ Mapping : public IMapping {
public:
        Mapping () : m2vEditor {nullptr}, v2mEditor {nullptr} {}
        virtual ~Mapping () {}

        virtual std::string getWidget () const { return widget; }
        virtual std::string getProperty () const { return property; }
        virtual std::string getModel () const { return model; }

        virtual ValidationAndBindingResult view2Model (MappingDTO *dto) __tiliae_no_reflect__;
        virtual void model2View (MappingDTO *dto) __tiliae_no_reflect__;

        static ValidationAndBindingResult view2Model (MappingDTO *dto, std::string const &widget, std::string const &property, std::string const &model, Editor::IEditor *editor = nullptr);
        static void model2View (MappingDTO *dto, std::string const &widget, std::string const &property, std::string const &model, Editor::IEditor *editor = nullptr);

protected:

        virtual void finalPropertyAndModel (std::string *finalProperty, std::string *finalModelName, MappingDTO *dto);

        virtual Core::Variant getFromModel (Wrapper::BeanWrapper *wrapper, Core::Variant objectToWrap, std::string const &finalModelName);
        virtual ValidationAndBindingResult setToModel (Wrapper::BeanWrapper *wrapper, Core::Variant objectToWrap, std::string const &finalModelName, Core::Variant valueToSet);
        virtual Core::Variant getFromView (ViewElementDTO *viewObject, std::string const &finalProperty);
        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

        virtual std::string getDefaultProperty (App *app, ViewElementDTO *dto) const;

public:

        std::string widget;
        std::string property;
        std::string model;
        Editor::IEditor *m2vEditor;
        Editor::IEditor *v2mEditor;
};

typedef __tiliae_reflect__ std::vector <IMapping *> MappingVector;

typedef std::unordered_map <std::string, IMapping *> MappingMap;
typedef std::unordered_multimap <std::string, IMapping *> MappingMultiMap;

} /* namespace GtkForms */

#endif /* MAPPING_H_ */
