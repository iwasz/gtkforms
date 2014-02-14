/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GOBJECT_WRAPPER_PLUGIN_H
#define GOBJECT_WRAPPER_PLUGIN_H

#include <Tiliae.h>

namespace GtkForms {

/**
 *  Plugin, ktory potrafi odczytywac i ustawiac za pomoca refleksji
 *  pola obiektow. Najpierw zprawdza, czy jest w klasie Field o danej
 *  nazwie, a jesli nie ma, szuka setterów i getterów.
 */
class GObjectWrapperPlugin : public Wrapper::IBeanWrapperPlugin {
public:

        virtual ~GObjectWrapperPlugin () {}

        virtual Core::Variant get (const Core::Variant &bean,
                                   Common::IPath *path,
                                   bool *error,
                                   Core::DebugContext *ctx = NULL,
                                   Editor::IEditor *editor = NULL) const;

        virtual bool set (Core::Variant *bean,
                          Common::IPath *path,
                          const Core::Variant &objectToSet,
                          Core::DebugContext *ctx = NULL,
                          Editor::IEditor *editor = NULL);

        virtual bool add (Core::Variant *,
                          Common::IPath *path,
                          const Core::Variant &,
                          Core::DebugContext *,
                          Editor::IEditor * = NULL) { throw Core::Exception ("GObjectWrapperPlugin::add is not supported."); }

        virtual Core::Variant iterator (const Core::Variant &bean,
                                        Common::IPath *path,
                                        bool *error,
                                        Core::DebugContext *ctx) const { throw Core::Exception ("GObjectWrapperPlugin::iterator is not supported."); }

private:

        Reflection::Class *getClass (const Core::Variant &bean, const Common::IPath *path) const;

};

} // namespace

#endif

