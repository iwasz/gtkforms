/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef DUMMYCONTROLLER_H_
#define DUMMYCONTROLLER_H_

#include "AbstractController.h"
#include <core/variant/Variant.h>
#include <string>

namespace GtkForms {

class __tiliae_reflect__ DummyController : public AbstractController {
public:
        virtual ~DummyController () {}

        ViewsToOpen onStart ()
        {
                if (!model.isNone ()) {
                        set ("model", model);
                        refresh ("");
                }
                return view;
        }

        void onSubmit () {}
        void onStop () {}

        Core::Variant getModel () const { return model; }
        void setModel (const Core::Variant &value) { model = value; }

        std::string const &getView () const { return view; }
        void setView (const std::string &value) { view = value; }

private:
        std::string view;
        Core::Variant model;
};

} /* namespace GtkForms */

#endif /* DUMMYCONTROLLER_H_ */
