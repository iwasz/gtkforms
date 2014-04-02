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

Core::Variant TextViewMapping::getFromView (ViewElementDTO *viewObject, std::string const &finalProperty)
{
        GtkTextBuffer *buffer = 0;

        if (GTK_TEXT_VIEW (viewObject->inputWidget)) {
                buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (viewObject->inputWidget));
        }
        else {
                throw Core::Exception ("TextViewMapping::getFromView : widget type not supported. Only GtkTextView is supported.");
        }

        if (!buffer) {
                throw Core::Exception ("TextViewMapping::getFromView : buffer inside GtkTextView is NULL.");
        }

        ViewElementDTO element {G_OBJECT (buffer)};
        return Mapping::getFromView (&element, finalProperty);
}

/*--------------------------------------------------------------------------*/

void TextViewMapping::setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet)
{
        GtkTextBuffer *buffer = 0;

        if (GTK_TEXT_VIEW (viewObject->inputWidget)) {
                buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (viewObject->inputWidget));
        }
        else {
                throw Core::Exception ("TextViewMapping::setToView : widget type not supported. Only GtkTextView is supported.");
        }

        if (!buffer) {
                throw Core::Exception ("TextViewMapping::setToView : buffer inside GtkTextView is NULL.");
        }

        ViewElementDTO element {G_OBJECT (buffer)};
        Mapping::setToView (&element, finalProperty, valueToSet);
}

} /* namespace GtkForms */
