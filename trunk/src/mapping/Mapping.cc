/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <beanWrapper/BeanWrapper.h>
#include "Mapping.h"
#include "Mapper.h"

namespace GtkForms {
using namespace Core;

void Mapping::m2v (MapperDTO *dto)
{
        Common::Context ctx;
        Variant modelProperty = dto->beanWrapper->get (modelProp, &ctx); // Discard errors

#if 0
        std::cerr << "Model property (" << (*i)->getModelProp () << ") : " << modelProperty << std::endl;
#endif

        if (modelProperty.isNone ()) {
                return;
        }

        dto->beanWrapper->set (viewProp, modelProperty, &ctx);

        if (ctx.isError ()) {
                throw MappingException ("Mapper::m2v bw->set : " + ctx.getMessage ());
        }
}

/****************************************************************************/

void Mapping::v2m (MapperDTO *dto)
{

}

}
