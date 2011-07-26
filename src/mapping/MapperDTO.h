/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef MAPPERDTO_H_
#define MAPPERDTO_H_

#include <cstddef>

namespace Wrapper {
class BeanWrapper;
}

namespace GtkForms {

/**
 * Everythong that mapper will need to perform its taks. Stis stuff is
 * set and passed by Mapper to every IMapping it runs.
 */
struct MapperDTO {

        MapperDTO () : beanWrapper (NULL) {}

        Wrapper::BeanWrapper *beanWrapper;
        Core::Variant model;

};

}

#	endif /* MAPPERDTO_H_ */
