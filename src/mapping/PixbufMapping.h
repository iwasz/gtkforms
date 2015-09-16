/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef RAWTOPIXBUF_H_
#define RAWTOPIXBUF_H_

#include <GtkForms.h>
#include "Mapping.h"
#include <vector>
#include "AssociationMap.h"

namespace GtkForms {

/**
 *
 */
class PixbufMapping : public Mapping {
public:
        ctr__ (void)
        bse_ ("Mapping")
        PixbufMapping () : maxWidth {-1}, maxHeight {-1} {}
        virtual ~PixbufMapping () {}

        virtual ValidationAndBindingResult view2Model (MappingDTO *dto) { /* Not implemented and won't be */ return ValidationAndBindingResult{}; }

protected:

        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

private:

        int prp_ (maxWidth);
        int prp_ (maxHeight);
        AssociationMap prr_ (dict);
        end_ (PixbufMapping)
};

} // end namespace

#endif /* RAWTOPIXBUF_H_ */
