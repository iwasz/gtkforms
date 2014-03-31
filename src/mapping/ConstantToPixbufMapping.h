/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef CONSTANTA_RAWTOPIXBUF_H_
#define CONSTANTA_RAWTOPIXBUF_H_

#include <GtkForms.h>
#include "IMapping.h"
#include <map>

namespace GtkForms {

typedef std::map <int, std::string> AssociationMap;
col_ (AssociationMap)

/**
 *
 */
class ConstantToPixbufMapping : public IMapping {
public:
        ctr__ (void)
        virtual ~ConstantToPixbufMapping () {}

        std::string getInput () const { return input; }

        void view2Model (MappingDTO *dto);
        void model2View (MappingDTO *dto);

private:
        std::string prp_ (input);
        std::string prp_ (model);
        AssociationMap prr_ (dict);
        end_ (ConstantToPixbufMapping)
};

} // end namespace

#endif /* RAWTOPIXBUF_H_ */
