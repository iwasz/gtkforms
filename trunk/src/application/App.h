/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef APP_H_
#define APP_H_

#include <Pointer.h>
#include <Reflection.h>
#include <ApiMacro.h>

class IUIFactory;

/**
 * Main object.
 */
class TILIAE_API App {
public:
        __c (void)

        virtual ~App () {}
         void run ();

/*--------------------------------------------------------------------------*/

        Ptr <IUIFactory> getFactory () const { return factory; }
        _s (setFactory) void setFactory (Ptr <IUIFactory> f) { factory = f; }

private:

        Ptr <IUIFactory> factory;

        _e (App)
};

#	endif /* APP_H_ */
