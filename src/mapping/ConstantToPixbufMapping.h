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
#include <map>
#include "Column.h"

namespace GtkForms {

typedef std::map <int, std::string> AssociationMap;
col_ (AssociationMap)

/**
 *
 */
class ConstantToPixbufMapping : public Column {
public:
        ctr__ (void)
        bse_ ("Column")
        virtual ~ConstantToPixbufMapping () {}
        virtual void view2Model (MappingDTO *dto) { /* Not implemented and won't be */ }

protected:

        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

private:

        AssociationMap prr_ (dict);
        end_ (ConstantToPixbufMapping)
};

} // end namespace

#endif /* RAWTOPIXBUF_H_ */
