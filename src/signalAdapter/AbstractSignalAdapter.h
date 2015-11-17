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
        virtual ~AbstractSignalAdapter () {}

        virtual std::string getSignal () const { return signal; }
        virtual std::string getGObjectName () const { return gObjectName; }
        virtual std::string getWidgetId () const { return widgetId; }

private:

        std::string signal;
        std::string gObjectName;
        std::string widgetId;
};

typedef __tiliae_reflect__ std::vector <ISignalAdapter *> SignalAdapterVector;

} /* namespace GtkForms */
#endif /* ABSTRACTSIGNALADAPTER_H_ */
