/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/
#ifndef IPAGEDECORATOR_H_
#define IPAGEDECORATOR_H_

namespace GtkForms {

/**
 * Some custom actions to be performed on a page/view.
 */
class IPageDecorator {
public:
        virtual ~IPageDecorator () {}
        virtual void run (Page *page, Context *ctx) = 0;
};

} /* namespace GtkForms */

#endif /* IPAGEDECORATOR_H_ */
