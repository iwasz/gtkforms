/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "PixbufMapping.h"
#include "RawData.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>

/*
 * https://developer.gnome.org/gdk-pixbuf/unstable//gdk-pixbuf-Image-Data-in-Memory.html#gdk-pixbuf-copy
 */
namespace GtkForms {
static src::logger_mt &lg = logger::get ();

void PixbufMapping::setToView (ViewElementDTO *viewObject, std::string const &, Core::Variant valueToSet)
{
        if (!GTK_IS_IMAGE (viewObject->inputWidget)) {
                throw Core::Exception ("RawToPixbufMapping::setToView : Could not conver inputWidget to to GtkImage.");
        }

        GtkImage *image = GTK_IMAGE (viewObject->inputWidget);

        if (valueToSet.isNull ()) {
                return;
        }

        GdkPixbuf *pixbuf = nullptr;
        GInputStream *stream = nullptr;

        if (ccast<RawData *> (valueToSet)) {
                RawData *data = vcast<RawData *> (valueToSet);

                if (data->empty ()) {
                        gtk_image_clear (image);
                        return;
                }

                stream = g_memory_input_stream_new_from_data (data->data (), data->size (), NULL);
                GError *e = nullptr;
                pixbuf = gdk_pixbuf_new_from_stream (stream, NULL, &e);

                if (!pixbuf) {
                        BOOST_LOG (lg) << data->size ();
                        throw Core::Exception ("RawToPixbufMapping::model2View : failed to create GtkPixbuf from RawData. Message : ["
                                               + std::string (e->message) + "]");
                }
        }
        else {
                std::string imageFilePath;

                if (!dict.empty ()) {
                        std::string index = lcast<std::string> (valueToSet);
                        AssociationMap::const_iterator it;
                        if ((it = dict.find (index)) == dict.end ()) {
                                BOOST_LOG (lg) << "Index : [" << index << "] has not been found in the PixbufMapping::dict";
                                return;
                        }

                        imageFilePath = it->second;
                }
                else {
                        imageFilePath = lcast<std::string> (valueToSet);
                }

                GError *e = nullptr;
                pixbuf = gdk_pixbuf_new_from_file (imageFilePath.c_str (), &e);

#if 0
                BOOST_LOG (lg) << "++++++  " << it->second.c_str ();
#endif

                if (!pixbuf) {
                        throw Core::Exception ("ConstantToPixbufMapping::model2View : failed to create GtkPixbuf from RawData. Message : ["
                                               + std::string (e->message) + "]");
                }
        }

        int oWidth = gdk_pixbuf_get_width (pixbuf);
        int oHeight = gdk_pixbuf_get_height (pixbuf);
        int mWidth = maxWidth;
        int mHeight = maxHeight;

        if ((mWidth < 0 && mHeight < 0) || (mWidth >= 0 && oWidth <= mWidth && mHeight >= 0 && oHeight <= mHeight)) {
                gtk_image_set_from_pixbuf (image, pixbuf);
                g_object_unref (pixbuf);
                return;
        }

        if (mHeight >= 0 && mHeight > oHeight) {
                double ratio = double(mHeight) / double(oHeight);
                mWidth = oWidth * ratio + 0.5;
        }

        if (mWidth >= 0 && mWidth > oWidth) {
                double ratio = double(mWidth) / double(oWidth);
                mHeight = oHeight * ratio + 0.5;
        }

        GdkPixbuf *scaled = gdk_pixbuf_scale_simple (pixbuf, mWidth, mHeight, GDK_INTERP_BILINEAR);
        g_object_unref (pixbuf);

        if (stream) {
                g_object_unref (stream);
        }

        gtk_image_set_from_pixbuf (image, scaled);
}

} // namespace
