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

namespace Catalog {

class EditController {
public:

        __c (void)
        EditController () : bookDAO (NULL) {}
        virtual ~EditController () {}

        _m (onFormShow) Ptr <Book> onFormShow() const;
        _m (onOk) void onOk(Ptr <Book> book);

        BookDAO *getBookDAO () const { return bookDAO; }
        _s (setBookDAO) void setBookDAO (BookDAO *bookDAO) { this->bookDAO = bookDAO; }

private:

        BookDAO *bookDAO;
        _e (EditController)
};

}

#	endif /* EDITCONTROLLER_H_ */
