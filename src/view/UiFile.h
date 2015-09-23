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
        REFLECTION_CONSTRUCTOR_ (void)

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

        std::string REFLECTION_FIELD_REFERENCE_INPLACE (file);

        class Impl;
        Impl *impl = 0;

        REFLECTION_END (UiFile)
};

} /* namespace GtkForms */
#endif /* UIFILE_H_ */
