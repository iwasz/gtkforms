/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef MAPPING_H_
#define MAPPING_H_

#include <ApiMacro.h>
#include <Reflection.h>
#include <string>
#include <vector>
#include "mapping/IMapping.h"
#include "mapping/MapperDTO.h"

namespace GtkForms {

/**
 *
 */
class TILIAE_API Mapping : public IMapping {
public:

        __c (void)
        virtual ~Mapping () {}

/*--------------------------------------------------------------------------*/

        virtual void m2v (MapperDTO *dto);
        virtual void v2m (MapperDTO *dto);

/*--------------------------------------------------------------------------*/

        std::string const &getModelProp () const { return modelProp; }
        _m (setModelProp) void setModelProp (std::string const &modelProp) { this->modelProp = modelProp; }

        std::string const &getViewProp () const { return viewProp; }
        _m (setViewProp) void setViewProp (std::string const &viewProp) { this->viewProp = viewProp; }

private:

        std::string viewProp;
        std::string modelProp;

        _e (Mapping)
};

}

#	endif /* MAPPING_H_ */
