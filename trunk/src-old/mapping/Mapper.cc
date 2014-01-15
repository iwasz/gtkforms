/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Mapper.h"

namespace GtkForms {
using namespace Core;

Core::Variant Mapper::m2v (Core::Variant const &model)
{
        if (model.isNone ()) {
                return Core::Variant ();
        }

        Wrapper::BeanWrapper *bw = myBeanWrapper ();
        Core::Variant domain = Core::Variant (context.get ());
        Common::Context ctx;

        // Ustaw, żeby można było potem mapować w drugą stronę.
        bw->setWrappedObject (domain);
        bw->set (getModelName (), model, &ctx);

        if (ctx.isError ()) {
                throw MappingException ("Mapper::m2v bw->set : " + ctx.getMessage ());
        }

        MapperDTO dto;
        dto.beanWrapper = bw;
        dto.model = model; // kopia

        for (MappingVector::const_iterator i = mappings->begin (); i != mappings->end (); ++i) {
                (*i)->m2v (&dto);
        }

        return model;
}

/****************************************************************************/

Core::Variant Mapper::v2m (Core::Variant const &view)
{
//        Wrapper::BeanWrapper *bw = myBeanWrapper ();
//        Core::Variant domain = Core::Variant (context.get ());
//        Common::Context ctx;
//        bw->setWrappedObject (domain);
//
//        for (MappingVector::const_iterator i = mappings->begin (); i != mappings->end (); ++i) {
//
//                Variant viewProperty = bw->get ((*i)->getViewProp (), &ctx); // Discard errors
//
//#if 0
//                std::cerr << "View property (" << (*i)->getViewProp () << ") : " << viewProperty << std::endl;
//#endif
//
//                if (viewProperty.isNone ()) {
//                        continue;
//                }
//
//                bw->set ((*i)->getModelProp (), viewProperty, &ctx);
//
//                if (ctx.isError ()) {
//                        throw MappingException ("Mapper::m2v bw->set : " + ctx.getMessage ());
//                }
//        }
//
//        Variant model = bw->get (getModelName (), &ctx);
//
//        if (ctx.isError ()) {
//                throw MappingException ("Mapper::m2v bw->get : " + ctx.getMessage ());
//        }
//
//        return model;
}

/****************************************************************************/

Wrapper::BeanWrapper *Mapper::myBeanWrapper () const
{
        if (!beanWrapper) {
                beanWrapper = Wrapper::BeanWrapper::create ();
        }

        return beanWrapper.get ();
}

} /* namespace GtkForms */
