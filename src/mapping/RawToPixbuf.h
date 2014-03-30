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
#include "IMapping.h"

namespace GtkForms {

/**
 *
 */
class RawToPixbuf : public IMapping {
public:
        ctr__ (void)
        RawToPixbuf ();
        virtual ~RawToPixbuf ();

        void view2Model (MappingDTO *dto);
        void model2View (MappingDTO *dto);

private:
        std::string prp_ (input);
        std::string prp_ (model);
        class Impl;
        Impl *impl;
        end_ (RawToPixbuf)
};

} // end namespace

#endif /* RAWTOPIXBUF_H_ */
