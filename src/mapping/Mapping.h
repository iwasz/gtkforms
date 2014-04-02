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
#include "ReflectionMacros.h"
#include <Tiliae.h>
#include "RawData.h"

namespace GtkForms {
class App;

class Mapping : public IMapping {
public:
        ctr__ (void)
        Mapping () : m2vEditor {nullptr}, v2mEditor {nullptr} {}
        virtual ~Mapping () {}

        virtual std::string getInput () const { return input; }
        virtual std::string getProperty () const { return property; }
        virtual std::string getModel () const { return model; }

        virtual void view2Model (MappingDTO *dto);
        virtual void model2View (MappingDTO *dto);

        static void view2Model (MappingDTO *dto, std::string const &input, std::string const &property, std::string const &model, Editor::IEditor *editor = nullptr);
        static void model2View (MappingDTO *dto, std::string const &input, std::string const &property, std::string const &model, Editor::IEditor *editor = nullptr);

protected:

        virtual void finalPropertyAndModel (std::string *finalProperty, std::string *finalModelName, MappingDTO *dto);

        virtual Core::Variant getFromModel (Wrapper::BeanWrapper *wrapper, Core::Variant objectToWrap, std::string const &finalModelName);
        virtual void setToModel (Wrapper::BeanWrapper *wrapper, Core::Variant objectToWrap, std::string const &finalModelName, Core::Variant valueToSet);
        virtual Core::Variant getFromView (GObject *viewObject, std::string const &finalProperty);
        virtual void setToView (GObject *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

        virtual std::string getDefaultProperty (App *app, std::string const &widgetClass) const;

private:

        std::string prp_ (input);
        std::string prp_ (property);
        std::string prp_ (model);
        Editor::IEditor *prp_ (m2vEditor);
        Editor::IEditor *prp_ (v2mEditor);

        end_ (Mapping)
};

typedef std::vector <IMapping *> MappingVector;
col_ (MappingVector)

typedef std::unordered_map <std::string, IMapping *> MappingMap;
typedef std::unordered_multimap <std::string, IMapping *> MappingMultiMap;

} /* namespace GtkForms */

#endif /* MAPPING_H_ */
