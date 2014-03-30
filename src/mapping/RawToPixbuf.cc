/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#include "RawToPixbuf.h"
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

/*
 * https://developer.gnome.org/gdk-pixbuf/unstable//gdk-pixbuf-Image-Data-in-Memory.html#gdk-pixbuf-copy
 */
namespace GtkForms {

struct RawToPixbuf::Impl {
        Impl ();
        ~Impl ();

        GdkPixbuf *pixbuf;
};

RawToPixbuf::Impl::Impl ()
{
        pixbuf = gdk_pixbuf_new ();
}

RawToPixbuf::RawToPixbuf ()
{
        impl = new Impl;
}

RawToPixbuf::~RawToPixbuf ()
{
        delete impl;
}

void RawToPixbuf::view2Model (MappingDTO *dto)
{
        // Not implemented and won't be.
        return;
}


void RawToPixbuf::model2View (MappingDTO *dto)
{

}

} //namespace
