/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef TABLEMAPPING_H_
#define TABLEMAPPING_H_

#include "IMapping.h"
#include "ReflectionMacros.h"
#include "Column.h"

namespace GtkForms {

/**
 * TODO Zrobić jedno z TextViewMapping i Mapping (konfigurowalny) i potem wydziedziczyć itak o.
 */
class TextViewMapping : public IMapping {
public:
        ctr__ (void)
        TextViewMapping () : property {"text"} {}
        virtual ~TextViewMapping () {}

        std::string getInput () const { return input; }

        void view2Model (MappingDTO *dto);
        void model2View (MappingDTO *dto);

private:

        std::string prp_ (input);
        std::string prp_ (property);
        std::string prp_ (model);

        end_ (TextViewMapping)
};

} /* namespace GtkForms */
#endif /* TABLEMAPPING_H_ */
