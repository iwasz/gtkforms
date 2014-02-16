/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <cassert>
#include <gtk/gtk.h>
#include "GObjectWrapperPlugin.h"
#include "GValueVariant.h"
#include "Logging.h"

/****************************************************************************/

namespace GtkForms {
static src::logger_mt& lg = logger::get ();
using Core::StringList;
using Core::Variant;
using Core::DebugContext;
using Core::VariantVector;
using Reflection::ReflectionTools;
using namespace Common;

/****************************************************************************/

Variant GObjectWrapperPlugin::get (const Variant &bean,
                                   IPath *path,
                                   bool *error,
                                   DebugContext *ctx,
                                   Editor::IEditor *) const
{
        if (!path) {
                throw Core::Exception ("GObjectWrapperPlugin::set path is NULL");
        }

#       if 0
        std::cerr << "--> " << __FILE__ << "," << __FUNCTION__ << " @ " << __LINE__ << " : " << "GObjectWrapperPlugin::get "
                << path << ", path.countSegments = " << path->countSegments () << std::endl;
#endif

        if (!ccast <GObject *> (bean)) {
                dcError (ctx, "GObjectWrapperPlugin::get (Path : '" + path->toString () + "'). Not a GObject *.");
                setError (error);
                return Variant ();
        }

        if (path->countSegments () != 1) {
                dcError (ctx, "GObjectWrapperPlugin::get (Path : '" + path->toString () + "'). Path has more than 1 segment. Only 1 segment allowed, which represents a GObject property.");
                setError (error);
                return Variant ();
        }

        GObject *object = vcast <GObject *> (bean);
        char const *param = path->getFirstSegment ().c_str ();
        GParamSpec *spec = g_object_class_find_property (G_OBJECT_GET_CLASS (object), param);

        if (!spec) {
                dcError (ctx, "GObjectWrapperPlugin::get (Path : '" + path->toString () + "'). Non-existent GObject property has been requested. GObject typeName : [" + G_OBJECT_TYPE_NAME (object) + "].");
                setError (error);
                return Variant ();
        }

        GType propType = spec->value_type;
        GValue propValue = {0};

        g_value_init (&propValue, propType);
        g_object_get_property (object, param, &propValue);
        Core::Variant v = gValueToVariant (&propValue);
        g_value_unset (&propValue);
        clearError (error);
        path->cutFirstSegment ();

        return v;
}

/****************************************************************************/

bool GObjectWrapperPlugin::set (Core::Variant *bean,
                                IPath *path,
                                const Core::Variant &objectToSet,
                                DebugContext *ctx,
                                Editor::IEditor *editor)
{
#if 0
        std::cerr << "--> " << __FILE__ << "," << __FUNCTION__ << " @ " << __LINE__ << " : " << "GObjectWrapperPlugin::get "
                << path << ", path.countSegments = " << path->countSegments () << std::endl;
#endif

        if (!path) {
                throw Core::Exception ("GObjectWrapperPlugin::set path is NULL");
        }

        if (!ccast <GObject *> (*bean)) {
                dcError (ctx, "GObjectWrapperPlugin::get (Path : '" + path->toString () + "'). Not a GObject *.");
                return false;
        }

        if (path->countSegments () != 1) {
                dcError (ctx, "GObjectWrapperPlugin::get (Path : '" + path->toString () + "'). Path has more than 1 segment. Only 1 segment allowed, which represents a GObject property.");
                return false;
        }

        GObject *object = vcast <GObject *> (*bean);
        GValue gVal = {0};
        Variant output = objectToSet;

        if (editor) {
                dcBegin (ctx);

                if (!editor->convert (objectToSet, &output, ctx)) {
                        dcError (ctx, "GObjectWrapperPlugin (Path : '" + path->toString () + "'). Editor failed.");
                        return false;
                }

                dcRollback (ctx);
        }

        variantToGValue (&gVal, output);
        char const *param = path->getFirstSegment ().c_str ();
        g_object_set_property (object, param, &gVal);
        g_value_unset (&gVal);
        path->cutFirstSegment ();
        return true;
}

} // namespace

