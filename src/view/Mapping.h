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

namespace GtkForms {

/**
 *
 */
class TILIAE_API Mapping {
public:

        __c (void)

        std::string const &getModelProp () const { return modelProp; }
        _m (setModelProp) void setModelProp (std::string const &modelProp) { this->modelProp = modelProp; }

        std::string const &getViewProp () const { return viewProp; }
        _m (setViewProp) void setViewProp (std::string const &viewProp) { this->viewProp = viewProp; }

private:

        std::string viewProp;
        std::string modelProp;

        _e (Mapping)
};

typedef std::vector <Ptr <Mapping> > MappingVector;
_g (MappingVector)

}

#	endif /* MAPPING_H_ */
