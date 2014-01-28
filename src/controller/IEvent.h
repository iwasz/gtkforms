/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef IEVENT_H_
#define IEVENT_H_

#include <stack>
#include <memory>

namespace GtkForms {

class App;

class IEvent {
public:
        virtual ~IEvent () {}
        virtual void run (App *app) = 0;
};

typedef std::stack <std::unique_ptr <IEvent>> EventStack;

} // namespace GtkForms

#endif /* IEVENT_H_ */
