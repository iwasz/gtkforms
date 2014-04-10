/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "AbstractController.h"
#include "App.h"
#include "Context.h"

namespace GtkForms {

void AbstractController::setToSessionScope (std::string const &path, Core::Variant v)
{
        app->getContext ().setToSessionScope (path, v);
}

void AbstractController::setToUnitScope (std::string const &path, Core::Variant v)
{
        app->getContext ().setToUnitScope (path, v);
}

void AbstractController::setToFlashScope (std::string const &path, Core::Variant v)
{
        app->getContext ().setToFlashScope (&getFlashScope (), path, v);
}

/*--------------------------------------------------------------------------*/

Core::Variant AbstractController::get (const std::string &name)
{
        return app->getContext ().get (name);
}

} // namespace GtkForms
