/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_TILEMANAGER_H_
#define GTK_FORMS_TILEMANAGER_H_

#include "IView.h"

namespace GtkForms {

class GtkTileManager {
public:

        void reparent (ViewMap *viewMap, bool show = true);

};

} /* namespace GtkForms */
#endif /* TILEMANAGER_H_ */
