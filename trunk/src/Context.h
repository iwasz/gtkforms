/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKFORMS_CONTEXT_H_
#define GTKFORMS_CONTEXT_H_

#include <Tiliae.h>
#include "ReflectionMacros.h"

namespace GtkForms {

class Context {
public:
        abt__

        virtual ~Context () {}

        Core::VariantMap &getSessionScope () { return session; }
        Core::VariantMap &getUnitScope () { return unit; }
        Core::VariantMap &getFlashScope () { return flash; }

        void clearSessionScope () { session.clear (); }
        void clearUnitScope () { unit.clear (); }
        void clearFlashScope () { flash.clear (); }

        mth_ (get) Core::Variant get (const std::string &name);

private:

        Core::VariantMap session;
        Core::VariantMap unit;
        Core::VariantMap flash;



        end_ (Context)
};

} // namespace GtkForms

#endif /* CONTEXT_H_ */
