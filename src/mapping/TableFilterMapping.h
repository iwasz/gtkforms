/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef TABLEFILTERMAPPING_H
#define TABLEFILTERMAPPING_H

#include "Mapping.h"
#include <ReflectionParserAnnotation.h>

namespace GtkForms {

class __tiliae_reflect__ TableFilterMapping : public Mapping {
public:
        TableFilterMapping ();
        virtual ~TableFilterMapping ();

//        std::string getWidget () const;
//        void setWidget (const std::string &value);

//        std::string getModel () const;
//        void setModel (const std::string &value);

        int getColumnNumber () const;
        void setColumnNumber (int value);

protected:

        virtual void setToView (ViewElementDTO *viewObject, std::string const &finalProperty, Core::Variant valueToSet);

private:
        struct Impl;
        Impl *impl;
};

} // namespace

#endif // TABLEFILTERMAPPING_H
