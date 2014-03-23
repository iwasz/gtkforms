/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef IMAPPING_H_
#define IMAPPING_H_

#include <string>
#include <core/Object.h>
#include <gtk/gtk.h>
#include "MappingDTO.h"

namespace GtkForms {
class App;
class Context;

class IMapping : public Core::Object {
public:
        virtual ~IMapping () {}

        /**
         * Name of input widget on the UI.
         */
        virtual std::string getInput () const = 0;

        virtual void view2Model (MappingDTO *dto) = 0;
        virtual void model2View (MappingDTO *dto) = 0;
};

} /* namespace GtkForms */

#endif /* IMAPPING_H_ */