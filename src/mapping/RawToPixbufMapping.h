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

namespace GtkForms {

/**
 *
 */
class RawToPixbufMapping : public Mapping {
public:
        ctr__ (void)
        bse_ ("Mapping")
        RawToPixbufMapping () : maxWidth {-1}, maxHeight {-1} {}
        virtual ~RawToPixbufMapping () {}

        virtual void view2Model (MappingDTO *dto) { /* Not implemented and won't be */ }

protected:

        virtual void setToView (GObject *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

private:

        int prp_ (maxWidth);
        int prp_ (maxHeight);
        end_ (RawToPixbufMapping)
};

} // end namespace

#endif /* RAWTOPIXBUF_H_ */
