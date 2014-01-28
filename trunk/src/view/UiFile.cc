/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "UiFile.h"

namespace GtkForms {

struct UiFile::Impl {
        GtkBuilder *builder = 0;
        bool loaded = false;
};

UiFile::UiFile ()
{
        impl = new Impl;
}

UiFile::~UiFile ()
{
        unload ();
        delete impl;
}

GtkBuilder *UiFile::load ()
{
        if (file.empty ()) {
                throw Core::Exception ("You didn't specify a file name to load in this UiFile factory object.");
        }

        GError *err = NULL;

        impl->builder = gtk_builder_new ();
        gtk_builder_add_from_file (impl->builder, file.c_str (), &err);

        if (err) {
                throw Core::Exception ("gtk_builder_add_from_file returned an error. Error message is : [" + std::string (err->message) + "]");
        }

        return impl->builder;
}

void UiFile::unload ()
{
        g_object_unref (impl->builder);
}

} /* namespace GtkForms */
