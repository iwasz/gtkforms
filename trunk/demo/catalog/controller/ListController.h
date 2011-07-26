/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef EDITCONTROLLER_H_
#define EDITCONTROLLER_H_

#include <Reflection.h>
#include "../model/Book.h"
#include "../dao/BookDAO.h"
#include "view/GObjectAdapter.h"
#include <ApiMacro.h>

namespace Wrapper {
class BeanWrapper;
}

namespace Catalog {

class ListController {
public:

        __c (void)
        ListController () : bookDAO (NULL) {}
        virtual ~ListController () {}

        _m (init) void init ();
        _m (onFormShow) Ptr <BookVector> onFormShow () const;
        _m (onOk) void onOk (Ptr <GtkForms::GObjectAdapter> widget, std::string const &name);
        _m (onTable) void onTable (Ptr <GtkForms::GObjectAdapter> table);

        BookDAO *getBookDAO () const { return bookDAO; }
        _s (setBookDAO) void setBookDAO (BookDAO *bookDAO) { this->bookDAO = bookDAO; }

private:

        BookDAO *bookDAO;

        _e (ListController)
};

}

#	endif /* EDITCONTROLLER_H_ */
