/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ABSTRACTSIGNALADAPTER_H_
#define ABSTRACTSIGNALADAPTER_H_

#include "ISignalAdapter.h"
#include "Logging.h"
#include <ReflectionParserAnnotation.h>
#include <string>

namespace GtkForms {

class __tiliae_reflect__ AbstractSignalAdapter : public ISignalAdapter {
public:
        virtual ~AbstractSignalAdapter () {}

        virtual std::string getSignal () const { return signal; }
        virtual std::string getGObjectName () const { return gObjectName; }
        virtual std::string getWidgetId () const { return widgetId; }

        void setSignal (const std::string &value) { signal = value; }
        void setGObjectName (const std::string &value) { gObjectName = value; }
        void setWidgetId (const std::string &value) { widgetId = value; }

private:
        std::string signal;
        std::string gObjectName;
        std::string widgetId;
};


} /* namespace GtkForms */
#endif /* ABSTRACTSIGNALADAPTER_H_ */
