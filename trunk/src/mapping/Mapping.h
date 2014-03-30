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

namespace GtkForms {
class App;

class Mapping : public IMapping {
public:
        ctr__ (void)
        Mapping () : m2vEditor {nullptr}, v2mEditor {nullptr} {}
        virtual ~Mapping () {}

        std::string getInput () const { return input; }

        void view2Model (MappingDTO *dto);
        void model2View (MappingDTO *dto);

        static void view2Model (MappingDTO *dto, std::string const &input, std::string const &property, std::string const &model, Editor::IEditor *editor = nullptr);
        static void model2View (MappingDTO *dto, std::string const &input, std::string const &property, std::string const &model, Editor::IEditor *editor = nullptr);

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
