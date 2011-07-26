/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "BookDAO.h"
#include "../model/Author.h"
#include "../model/Publisher.h"
#include "../model/Book.h"

namespace Catalog {

Ptr <Book> BookDAO::createTestData () const
{
        // To że niby pobieramy z bazy danych, albo coś:
        Ptr <Author> author = boost::make_shared <Author> ();
        Ptr <Publisher> publisher = boost::make_shared <Publisher> ();
        Ptr <Book> book = boost::make_shared <Book> ();

        book->setTitle ("Rany wylotowe");
        book->setIsbn ("978-83-60915-42-4");
        book->setYear (2010);
        book->setPublisher (publisher);
        publisher->setName ("Kultura Gniewu");
        book->setAuthor (author);
        author->setFirstname ("Rutu");
        author->setLastname ("Modan");

        return book;
}

/****************************************************************************/

Ptr <Book> BookDAO::getBook (unsigned int id) const
{
        return createTestData ();
}

/****************************************************************************/

Ptr <BookVector> BookDAO::getBooks () const
{
        Ptr <BookVector> ret = boost::make_shared <BookVector> ();

        Ptr <Book> book = createTestData ();
        ret->push_back (book);

        book = createTestData ();
        book->setTitle ("Mikropolis");
        ret->push_back (book);

        book = createTestData ();
        book->setTitle ("Niebieskie pigułki");
        ret->push_back (book);

        book = createTestData ();
        book->setTitle ("Deszcz");
        ret->push_back (book);

        return ret;
}

} /* namespace Catalog */
