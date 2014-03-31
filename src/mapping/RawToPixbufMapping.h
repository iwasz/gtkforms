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
#include <vector>

namespace GtkForms {

/**
 *
 */
class RawToPixbufMapping : public IMapping {
public:
        ctr__ (void)
        RawToPixbufMapping () : maxWidth {-1}, maxHeight {-1}/*, expand {false}*/ {}
        virtual ~RawToPixbufMapping () {}

        std::string getInput () const { return input; }

        void view2Model (MappingDTO *dto);
        void model2View (MappingDTO *dto);

private:
        std::string prp_ (input);
        std::string prp_ (model);
        int prp_ (maxWidth);
        int prp_ (maxHeight);
//        bool prp_ (expand);
        end_ (RawToPixbufMapping)
};

} // end namespace

#endif /* RAWTOPIXBUF_H_ */
