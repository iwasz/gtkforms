/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef DEFAULTQUITHANDLER_H_
#define DEFAULTQUITHANDLER_H_

#include "IHandler.h"

namespace GtkForms {

class DefaultQuitHandler : public IHandler {
public:
        virtual ~DefaultQuitHandler () {}
        void run (App *app);
};

} /* namespace GtkForms */

#endif /* DEFAULTQUITHANDLER_H_ */
