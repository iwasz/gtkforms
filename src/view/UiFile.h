/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GTK_FORMS_UIFILE_H_
#define GTK_FORMS_UIFILE_H_

#include <gtk/gtk.h>
#include "ReflectionMacros.h"

namespace GtkForms {
class Context;
class App;

/**
 * Single GTK+ *.ui file. GtkBuilder is embedded in this class.
 */
class UiFile {
public:
        ctr__ (void)

        UiFile ();
        ~UiFile ();

        /**
         * Loads file into memory, and returns it (in form of GtkBuilder). If file is allready loaded,
         * it is immediately returned.
         */
        GtkBuilder *load (App *app);
        GtkBuilder *getBuilder ();

        /**
         * Clears memory occupied by loaded UI file.
         */
        void destroy ();

        std::string getFile () const { return file; }
        void setFile (std::string const &f) { file = f; }

private:

        void connectSignals (App *app);

private:

        std::string prr_ (file);

        class Impl;
        Impl *impl = 0;

        end_ (UiFile)
};

} /* namespace GtkForms */
#endif /* UIFILE_H_ */
