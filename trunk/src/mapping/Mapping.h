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

namespace GtkForms {

class Mapping : public IMapping {
public:
        ctr__ (void)

        virtual ~Mapping () {}

        std::string getInput () const { return input; }
        std::string getProperty () const { return property; }
        std::string getModel () const { return model; }

private:

        std::string prp_ (input);
        std::string prp_ (property);
        std::string prp_ (model);
        end_ (Mapping)
};

typedef std::vector <IMapping *> MappingVector;
col_ (MappingVector)

typedef std::unordered_map <std::string, IMapping *> MappingMap;
typedef std::unordered_multimap <std::string, IMapping *> MappingMultiMap;

} /* namespace GtkForms */

#endif /* MAPPING_H_ */
