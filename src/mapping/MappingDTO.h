/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef MAPPINGDTO_H_
#define MAPPINGDTO_H_

#include <Tiliae.h>
#include <gtk/gtk.h>
#include <string>

namespace GtkForms {
class App;
class Context;

struct ViewElementDTO {
        ViewElementDTO (GObject *i) : inputWidget{ i } {}
        virtual ~ViewElementDTO () {}

        GObject *inputWidget = 0;
};

/**
 * Helper class to pass data between App and mappers.
 */
struct MappingDTO {
        App *app = 0;
        ViewElementDTO *viewElement = 0;
        Core::Variant m2vModelObject;
        Core::Variant v2mModelObject;

        /// modelRange or inputRange - depends on what event is processed.
        // std::string dataRange;
};

} /* namespace GtkForms */
#endif /* MAPPINGDTO_H_ */
