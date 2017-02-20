/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef KEYPRESSEVENTADAPTER_H
#define KEYPRESSEVENTADAPTER_H

#include "AbstractSignalAdapter.h"

namespace GtkForms {

class __tiliae_reflect__ KeyPressEventAdapter : public AbstractSignalAdapter {
public:
        virtual ~KeyPressEventAdapter () {}
        Core::VariantVector adapt (guint n_param_values, const GValue *param_values) const;
        virtual std::string getSignal () const { return "key-press-event"; }

};

} // namespace

#endif // KEYPRESSEVENTADAPTER_H
