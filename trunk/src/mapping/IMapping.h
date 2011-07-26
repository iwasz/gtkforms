/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef IMAPPING_H_
#define IMAPPING_H_

#include <Object.h>
#include <ApiMacro.h>
#include <vector>
#include <Reflection.h>
#include "mapping/MapperDTO.h"
#include <Pointer.h>

namespace GtkForms {

struct TILIAE_API IMapping : public Core::Object {

        virtual ~IMapping () {}
        virtual void m2v (MapperDTO *dto) = 0;
        virtual void v2m (MapperDTO *dto) = 0;
};

typedef std::vector <Ptr <IMapping> > MappingVector;
_g (MappingVector)

}

#	endif /* IMAPPING_H_ */
