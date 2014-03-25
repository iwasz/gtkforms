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
 * IMapping which converts data between the model and the view designed to be used with GtkAdjustment.
 * First thing this mapper tries is to cast MappingDTO::inputWidget (widget from the value is to be retreived)
 * to some concrete type which uses GtkAdjustment (such as GtkScale or GtkSpinButton).
 *
 * TODO Zrobić jedno z AdjustmentMapping i Mapping (konfigurowalny) i potem wydziedziczyć itak o.
 */
class AdjustmentMapping : public IMapping {
public:
        ctr__ (void)
        virtual ~AdjustmentMapping () {}

        std::string getInput () const { return input; }

        void view2Model (MappingDTO *dto);
        void model2View (MappingDTO *dto);

private:

        std::string prp_ (input);
        std::string prp_ (property);
        std::string prp_ (model);

        end_ (AdjustmentMapping)
};

} /* namespace GtkForms */
#endif /* TABLEMAPPING_H_ */
