/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "AdjustmentMapping.h"
#include "Logging.h"
#include "App.h"
#include "Context.h"
#include <gtk/gtk.h>
#include <Tiliae.h>
#include "GValueVariant.h"

namespace GtkForms {
static src::logger_mt& lg = logger::get();

Core::Variant AdjustmentMapping::getFromView (GObject *viewObject, std::string const &finalProperty)
{
        GtkAdjustment *adjustment = 0;

        if (GTK_IS_RANGE (viewObject)) {
                adjustment = gtk_range_get_adjustment (GTK_RANGE (viewObject));
        }
        else if (GTK_IS_SPIN_BUTTON (viewObject)) {
                adjustment = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (viewObject));
        }
        else {
                throw Core::Exception ("AdjustmentMapping::getFromView : widget type not supported. Supported types : GtkSpinButton, GtkRange.");
        }

        if (!adjustment) {
                throw Core::Exception ("AdjustmentMapping::getFromView : adjustment is NULL.");
        }

        return Mapping::getFromView (G_OBJECT (adjustment), finalProperty);
}

/*--------------------------------------------------------------------------*/

void AdjustmentMapping::setToView (GObject *viewObject, std::string const &finalProperty, Core::Variant valueToSet)
{
        GtkAdjustment *adjustment = 0;

        if (GTK_IS_RANGE (viewObject)) {
                adjustment = gtk_range_get_adjustment (GTK_RANGE (viewObject));
        }
        else if (GTK_IS_SPIN_BUTTON (viewObject)) {
                adjustment = gtk_spin_button_get_adjustment (GTK_SPIN_BUTTON (viewObject));
        }
        else {
                throw Core::Exception ("AdjustmentMapping::setToView : widget type not supported. Supported types : GtkSpinButton, GtkRange.");
        }

        if (!adjustment) {
                throw Core::Exception ("AdjustmentMapping::setToView : adjustment is NULL.");
        }

        Mapping::setToView (G_OBJECT (adjustment), finalProperty, valueToSet);
}

} /* namespace GtkForms */
