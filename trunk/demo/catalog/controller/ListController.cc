/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "ListController.h"
#include "../model/Model.h"
#include <beanWrapper/BeanWrapper.h>
#include "tools/GValueVariant.h"

namespace Catalog {

void ListController::init ()
{
#if 1
        std::cerr << "ListController::init" << std::endl;
#endif
}

/****************************************************************************/

Ptr <BookVector> ListController::onFormShow () const
{
#if 1
        std::cerr << "ListController::onFormShow" << std::endl;
#endif

        return bookDAO->getBooks ();
}

/****************************************************************************/

void ListController::onOk (Ptr <GtkForms::GObjectAdapter> widget, std::string const &name)
{
#if 1
        std::cerr << "ListController::onOk (" << name << ")" << std::endl;
#endif

        GObject *object = widget->getGObject ();
        GtkWidget *gwidget = GTK_WIDGET (object);

#if 1
        std::cerr << "ListController::onOk (" << gtk_widget_get_name (gwidget) << ")" << std::endl;
#endif
}

/****************************************************************************/

void ListController::onTable (Ptr <GtkForms::GObjectAdapter> table)
{
#if 1
        std::cerr << "ListController::onTable ()" << std::endl;
#endif
}

} /* namespace Catalog */
