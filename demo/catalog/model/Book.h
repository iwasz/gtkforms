/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef BOOK_H_
#define BOOK_H_

#include <Pointer.h>
#include <Reflection.h>
#include <IToStringEnabled.h>
#include "Publisher.h"
#include "Author.h"

namespace Catalog {

class Book : public Core::IToStringEnabled {
public:

        __c (void)
        virtual ~Book () {}

        _m (getIsbn) std::string const &getIsbn () const { return isbn; }
        _m (setIsbn) void setIsbn (std::string const &isbn) { this->isbn = isbn; }

        _m (getTitle) std::string const &getTitle () const { return title; }
        _m (setTitle) void setTitle (std::string const &title) { this->title = title; }

        _m (getYear) unsigned int getYear () const { return year; }
        _m (setYear) void setYear (unsigned int year) { this->year = year; }

        _m (getPublisher) Ptr <Publisher> getPublisher () const { return publisher; }
        _m (setPublisher) void setPublisher (Ptr <Publisher> publisher) { this->publisher = publisher; }

        _m (getAuthor) Ptr <Author> getAuthor () const { return author; }
        _m (setAuthor) void setAuthor (Ptr <Author> author) { this->author = author; }

        std::string toString () const
        {
                return "Book (title=" + title + ", isbn=" + isbn + ", author=" +
                                author->toString () + ", publisher=" + publisher->toString () + ")";
        }

private:

        std::string title;
        std::string isbn;
        unsigned int year;
        Ptr <Publisher> publisher;
        Ptr <Author> author;
        _e (Book)
};

}

#	endif /* BOOK_H_ */
