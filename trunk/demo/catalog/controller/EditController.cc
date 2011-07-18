/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "EditController.h"
#include "../model/Model.h"

namespace Catalog {

Ptr <Book> EditController::onFormShow () const
{
#if 1
        std::cerr << "EditController::onFormShow" << std::endl;
#endif

        return bookDAO->getBook (0);
}

/****************************************************************************/

void EditController::onOk (Ptr <Book> book)
{
#if 1
        std::cerr << "EditController::onOk (" << book->toString () << ")" << std::endl;
#endif
}

} /* namespace Catalog */
