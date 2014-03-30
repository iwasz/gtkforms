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

        // TODO Tu jest błąd, który jak się naprawi, to program nie działa....
        Core::VariantMap &getSessionScope () { return session; }
        Core::VariantMap &getUnitScope () { return session; }
        Core::VariantMap &getFlashScope () { return session; }

        void clearSessionScope () { session.clear (); }
        void clearUnitScope () { session.clear (); }
        void clearFlashScope () { session.clear (); }

        mth_ (get) Core::Variant get (const std::string &name);

private:

        Core::VariantMap session;
        Core::VariantMap unit;
        Core::VariantMap flash;



        end_ (Context)
};

} // namespace GtkForms

#endif /* CONTEXT_H_ */
