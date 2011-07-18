/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef MAPPER_H_
#define MAPPER_H_

#include <ApiMacro.h>
#include <Reflection.h>
#include <Pointer.h>
#include <beanWrapper/BeanWrapper.h>
#include <variant/Cast.h>
#include "Mapping.h"
#include "../Defs.h"

namespace GtkForms {

class TILIAE_API Mapper {
public:

        __c (void)

        Mapper () : modelName (DEFAULT_MODEL_NAME) {}
        virtual ~Mapper () {}

        /**
         * Converts model properties into view properties upon information from
         * mappings vector. Expects that both model and view paths are valid. Stores
         * model in context map under key modelName.
         * @return Model.
         */
        _m (m2v) Core::Variant m2v (Core::Variant const &model);

        /**
         * Converts model properties into view properties upon information from
         * mappings vector. Expects that both model and view paths are valid. Stores
         * model in context map under key modelName.
         * @return Converted model.
         */
        _m (v2m) Core::Variant v2m (Core::Variant const &view);

/*--------------------------------------------------------------------------*/

        /**
         * Name of key where model will be stored. Model is stored in context
         * by m2v method.
         */
        std::string const &getModelName () const { return modelName; }
        _m (setModelName) void setModelName (std::string const &modelName) { this->modelName = modelName; }

        /**
         * Map which contains all surrent elements of MVC structure ie. view, controller and
         * (not necessarily) a model.
         */
        Ptr <Core::VariantMap> getContext () const { return context; }
        _m (setContext) void setContext (Ptr <Core::VariantMap> c) { context = c; }

        /**
         * Bean wrapper which will be used for transfering model properties
         * to view properties and back. If none is set, BeanWrapper::create
         * will be used.
         */
        Ptr<Wrapper::BeanWrapper> getBeanWrapper () const { return beanWrapper; }
        _m (setBeanWrapper) void setBeanWrapper (Ptr<Wrapper::BeanWrapper> beanWrapper) { this->beanWrapper = beanWrapper; }

        /**
         * View <-> Model mappings.
         */
        Ptr <MappingVector> getMappings () const { return mappings; }
        _s (setMappings) void setMappings (Ptr <MappingVector> mappings) { this->mappings = mappings; }

private:

        Wrapper::BeanWrapper *myBeanWrapper () const;

private:

        std::string modelName;
        Ptr <MappingVector> mappings;
        Ptr <Core::VariantMap> context;
        mutable Ptr <Wrapper::BeanWrapper> beanWrapper;

        _e (Mapper)

};

/**
 *
 */
struct TILIAE_API MappingException : public Core::Exception {
        MappingException (std::string const &m = "") : Core::Exception (m) {}
        virtual ~MappingException () throw () {}
};

}

#	endif /* MAPPER_H_ */
