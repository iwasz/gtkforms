/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "TextViewMapping.h"
#include "Logging.h"
#include "App.h"
#include "Context.h"
#include <gtk/gtk.h>
#include <Tiliae.h>
#include "GValueVariant.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get();

Core::Variant TextViewMapping::getFromView (GObject *viewObject, std::string const &finalProperty)
{
        GtkTextBuffer *buffer = 0;

        if (GTK_TEXT_VIEW (viewObject)) {
                buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (viewObject));
        }
        else {
                throw Core::Exception ("TextViewMapping::getFromView : widget type not supported. Only GtkTextView is supported.");
        }

        if (!buffer) {
                throw Core::Exception ("TextViewMapping::getFromView : buffer inside GtkTextView is NULL.");
        }

        return Mapping::getFromView (G_OBJECT (buffer), finalProperty);
}

/*--------------------------------------------------------------------------*/

void TextViewMapping::setToView (GObject *viewObject, std::string const &finalProperty, Core::Variant valueToSet)
{
        GtkTextBuffer *buffer = 0;

        if (GTK_TEXT_VIEW (viewObject)) {
                buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (viewObject));
        }
        else {
                throw Core::Exception ("TextViewMapping::setToView : widget type not supported. Only GtkTextView is supported.");
        }

        if (!buffer) {
                throw Core::Exception ("TextViewMapping::setToView : buffer inside GtkTextView is NULL.");
        }

        Mapping::setToView (G_OBJECT (buffer), finalProperty, valueToSet);
}

} /* namespace GtkForms */
