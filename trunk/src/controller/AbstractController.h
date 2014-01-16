/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ABSTRACTCONTROLLER_H_
#define ABSTRACTCONTROLLER_H_

#include "IController.h"

class AbstractController : public IController {
public:
        AbstractController ();
        virtual ~AbstractController ();

        App *getApp () { return app; }
        void setApp (App *app) { this->app = app; }

private:

        App *app = 0;
};

#endif /* ABSTRACTCONTROLLER_H_ */
