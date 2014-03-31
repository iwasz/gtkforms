/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "RawToPixbufMapping.h"
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "RawData.h"
#include "Context.h"

/*
 * https://developer.gnome.org/gdk-pixbuf/unstable//gdk-pixbuf-Image-Data-in-Memory.html#gdk-pixbuf-copy
 */
namespace GtkForms {
static src::logger_mt& lg = logger::get();

void RawToPixbufMapping::view2Model (MappingDTO *dto)
{
        // Not implemented and won't be.
        return;
}

void RawToPixbufMapping::model2View (MappingDTO *dto)
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
        wrapper->setWrappedObject (Core::Variant (dto->context));
        Core::Variant v = wrapper->get (finalModelName);

        if (v.isNone ()) {
                return;
        }

        RawData *data = vcast <RawData *> (v);

        if (data->empty ()) {
                gtk_image_clear (image);
                return;
        }

        // TODO memory deallocation?
        GInputStream *stream = g_memory_input_stream_new_from_data (data->data (), data->size (), NULL);
        GError *e = nullptr;
        // TODO memory deallocation?
        GdkPixbuf *pixbuf = gdk_pixbuf_new_from_stream (stream, NULL, &e);

        if (!pixbuf) {
                throw Core::Exception ("RawToPixbuf::model2View : failed to create GtkPixbuf from RawData. Message : [" + std::string (e->message) + "]");
        }

        int oWidth = gdk_pixbuf_get_width (pixbuf);
        int oHeight = gdk_pixbuf_get_height (pixbuf);
        int mWidth = maxWidth;
        int mHeight = maxHeight;

        if ((mWidth < 0 && mHeight < 0) || (mWidth >= 0 && oWidth <= mWidth && mHeight >= 0 && oHeight <= mHeight)) {
                gtk_image_set_from_pixbuf (image, pixbuf);
                return;
        }

        if (mHeight >= 0 && mHeight > oHeight) {
                double ratio = double (mHeight) / double (oHeight);
                mWidth = oWidth * ratio + 0.5;
        }

        if (mWidth >= 0 && mWidth > oWidth) {
                double ratio = double (mWidth) / double (oWidth);
                mHeight = oHeight * ratio + 0.5;
        }

        GdkPixbuf *scaled = gdk_pixbuf_scale_simple (pixbuf, mWidth, mHeight, GDK_INTERP_BILINEAR);
        g_object_unref (pixbuf);
        g_object_unref (stream);

        gtk_image_set_from_pixbuf (image, scaled);
}

} //namespace
