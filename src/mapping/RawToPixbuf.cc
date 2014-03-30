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
#include "RawData.h"
#include "Context.h"

/*
 * https://developer.gnome.org/gdk-pixbuf/unstable//gdk-pixbuf-Image-Data-in-Memory.html#gdk-pixbuf-copy
 */
namespace GtkForms {
static src::logger_mt& lg = logger::get();

struct RawToPixbuf::Impl {
        Impl ();
        ~Impl () {}

        GdkPixbuf *pixbuf;
};

RawToPixbuf::Impl::Impl ()
{
//        pixbuf = gdk_pixbuf_new ();
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
        if (!GTK_IS_IMAGE (dto->inputWidget)) {
                throw Core::Exception ("RawToPixbuf::model2View : Could not conver inputWidget to to GtkImage.");
        }

        GtkImage *image = GTK_IMAGE (dto->inputWidget);

        Wrapper::BeanWrapper *wrapper = dto->app->getBeanWrapper ();

        std::string finalModelName;

        if (!model.empty ()) {
                finalModelName = model;
        }
        else {
                finalModelName = input;
        }

        BOOST_LOG (lg) << finalModelName;
        // TODO Tak też nie działa, a powinno!
        wrapper->setWrappedObject (Core::Variant (&dto->context));

//        Core::VariantMap &unitScope = dto->context->getUnitScope ();
//        wrapper->setWrappedObject (Core::Variant (&unitScope));

        Core::Variant v = wrapper->get (finalModelName);

        if (v.isNone ()) {
                return;
        }

        RawData *data = vcast <RawData *> (v);
        GInputStream *stream = g_memory_input_stream_new_from_data (data->data (), data->size (), NULL);
        GError *error;
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_stream (stream, NULL, &error);

        if (!pixbuf) {
                throw Core::Exception ("RawToPixbuf::model2View : failed to create GtkPixbuf from RawData. Message : [" + std::string (error->message) + "]");
        }

        gtk_image_set_from_pixbuf (image, pixbuf);
}

} //namespace
