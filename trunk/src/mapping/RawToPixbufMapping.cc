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

/*
 * https://developer.gnome.org/gdk-pixbuf/unstable//gdk-pixbuf-Image-Data-in-Memory.html#gdk-pixbuf-copy
 */
namespace GtkForms {
static src::logger_mt& lg = logger::get();

void RawToPixbufMapping::setToView (GObject *viewObject, std::string const &, Core::Variant valueToSet)
{
        if (!GTK_IS_IMAGE (viewObject)) {
                throw Core::Exception ("TextViewMapping::setToView : Could not conver inputWidget to to GtkImage.");
        }

        GtkImage *image = GTK_IMAGE (viewObject);
        RawData *data = vcast <RawData *> (valueToSet);

        if (data->empty ()) {
                gtk_image_clear (image);
                return;
        }

        GInputStream *stream = g_memory_input_stream_new_from_data (data->data (), data->size (), NULL);
        GError *e = nullptr;
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
