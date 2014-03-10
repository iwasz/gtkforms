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
#include "ReflectionMacros.h"
#include "ISignalAdapter.h"
#include <string>

namespace GtkForms {

class AbstractSignalAdapter : public ISignalAdapter {
public:
        abt__
        virtual ~AbstractSignalAdapter () {}

        virtual std::string getSignal () const { return signal; }
        virtual std::string getGObjectName () const { return gObjectName; }
        virtual std::string getWidgetId () const { return widgetId; }

private:

        std::string prp_ (signal);
        std::string prp_ (gObjectName);
        std::string prp_ (widgetId);
        end_ (AbstractSignalAdapter)
};

typedef std::vector <ISignalAdapter *> SignalAdapterVector;
col_ (SignalAdapterVector)

} /* namespace GtkForms */
#endif /* ABSTRACTSIGNALADAPTER_H_ */
