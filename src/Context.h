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

namespace GtkForms {

class Context {
public:

        virtual ~Context () {}

        Core::VariantMap &getSessionScope () { return session; }
        Core::VariantMap &getUnitScope () { return session; }
        Core::VariantMap &getFlashScope () { return session; }

        void clearSessionScope () { session.clear (); }
        void clearUnitScope () { session.clear (); }
        void clearFlashScope () { session.clear (); }

        Core::Variant get (const std::string &name);

private:

        Core::VariantMap session;
        Core::VariantMap unit;
        Core::VariantMap flash;
};

} // namespace GtkForms

#endif /* CONTEXT_H_ */
