/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKBUILDERADAPTER_H_
#define GTKBUILDERADAPTER_H_

#include <gtk/gtk.h>
#include <cstddef>
#include <core/ApiMacro.h>
#include <reflection/Reflection.h>

namespace GtkForms {

/**
 * Wrapper for GtkBuilder. GtkBuilder instance holds all data gathered from
 * various *.ui files, thus it shoud be shared across multiple GtkBuilderFactories,
 * which adds new ui files descriptions to it.
 */
class TILIAE_API GtkBuilderAdapter {
public:

        GtkBuilderAdapter ();
        virtual ~GtkBuilderAdapter ();

        GtkBuilder *getBuilder () const { return builder; }

private:

        GtkBuilder *builder;

        __e (GtkBuilderAdapter)
};

}

#	endif /* GTKBUILDERADAPTER_H_ */
