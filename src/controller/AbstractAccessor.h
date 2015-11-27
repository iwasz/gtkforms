/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ABSTRACTACCESSOR_H
#define ABSTRACTACCESSOR_H

#include <ReflectionParserAnnotation.h>
#include <core/Typedefs.h>
#include "controller/AbstractController.h"

namespace GtkForms {
class AbstractController;


/**
 * @brief The AbstractAccessor class is used with BeanWrapper in various methods of Controller and
 * View.
 */
class __tiliae_reflect__ AbstractAccessor {
public:

        AbstractAccessor (Core::VariantMap *s = nullptr) : sessionScope {s} {}
        virtual ~AbstractAccessor () {}
        virtual Core::Variant get (const std::string &name) = 0;
        virtual void set (const std::string &name, Core::Variant v) = 0;

        void setSessionScope (Core::VariantMap *s) { sessionScope = s; }
        void setCurrentController (AbstractController *c) { currentController = c; }

protected:

        Core::VariantMap *sessionScope = nullptr;
        AbstractController *currentController = nullptr;

};

}

#endif // ABSTRACTACCESSOR_H
