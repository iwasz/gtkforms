/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef MAPPINGDTO_H_
#define MAPPINGDTO_H_

#include <string>
#include <gtk/gtk.h>
#include <Tiliae.h>

namespace GtkForms {
class  App;
class Context;

/**
 * Helper class to pass data between App and mappers.
 */
struct MappingDTO {
        App *app = 0;
        GObject *inputWidget = 0;
//        Context *context = 0;
        Core::Variant m2vModelObject;
        Core::Variant v2mModelObject;
        std::string dataRange;
};

} /* namespace GtkForms */
#endif /* MAPPINGDTO_H_ */
