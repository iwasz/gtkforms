/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ISIGNALADAPTER_H_
#define ISIGNALADAPTER_H_

#include <gtk/gtk.h>
#include <vector>
#include <core/Object.h>
#include <core/Typedefs.h>
#include <ReflectionParserAnnotation.h>

namespace GtkForms {

/**
 *
 */
struct __tiliae_reflect__ ISignalAdapter : public Core::Object {
        virtual ~ISignalAdapter () {}
        virtual std::string getSignal () const = 0;
        virtual std::string getGObjectName () const = 0;
        virtual std::string getWidgetId () const = 0;
        virtual Core::VariantVector adapt (guint n_param_values, const GValue *param_values) const = 0;
};

} /* namespace GtkForms */
#endif /* ISIGNALADAPTER_H_ */
