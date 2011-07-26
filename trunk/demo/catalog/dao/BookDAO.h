/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef BOOKDAO_H_
#define BOOKDAO_H_

#include <Reflection.h>
#include <Pointer.h>
#include "../model/Book.h"

namespace Catalog {

class BookDAO {
public:

        virtual ~BookDAO () {}

        Ptr <Book> getBook (unsigned int id) const;
        Ptr <BookVector> getBooks () const;

private:

        Ptr <Book> createTestData () const;

        __e (BookDAO)
};

}

#	endif /* BOOKDAO_H_ */
