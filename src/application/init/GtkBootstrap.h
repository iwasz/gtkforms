/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTKBOOTSTRAP_H_
#define GTKBOOTSTRAP_H_

#include <ApiMacro.h>

namespace GtkForms {

/**
 * Used to init gtk before any other classes get instantiated.
 */
class TILIAE_API GtkBootstrap {
public:

        static void init (int *argc, char ***argv);

};

}

#	endif /* GTKBOOTSTRAP_H_ */
