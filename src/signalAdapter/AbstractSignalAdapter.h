/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ABSTRACTSIGNALADAPTER_H_
#define ABSTRACTSIGNALADAPTER_H_

#include "Logging.h"
#include <ReflectionParserAnnotation.h>
#include "ISignalAdapter.h"
#include <string>

namespace GtkForms {

class __tiliae_reflect__ AbstractSignalAdapter : public ISignalAdapter {
public:
        REFLECTION_CLASS
        virtual ~AbstractSignalAdapter () {}

        virtual std::string getSignal () const { return signal; }
        virtual std::string getGObjectName () const { return gObjectName; }
        virtual std::string getWidgetId () const { return widgetId; }

private:

        std::string REFLECTION_FIELD_VALUE_INPLACE (signal);
        std::string REFLECTION_FIELD_VALUE_INPLACE (gObjectName);
        std::string REFLECTION_FIELD_VALUE_INPLACE (widgetId);
        REFLECTION_END (AbstractSignalAdapter)
};

typedef std::vector <ISignalAdapter *> SignalAdapterVector;
REFLECTION_COLLECTION (SignalAdapterVector)

} /* namespace GtkForms */
#endif /* ABSTRACTSIGNALADAPTER_H_ */
