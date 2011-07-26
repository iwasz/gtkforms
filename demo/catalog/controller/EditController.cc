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

//Core::VariantMap EditController::onFormShow () const
//{
//#if 1
//        std::cerr << "EditController::onFormShow" << std::endl;
//#endif
//
//        Core::VariantMap ret;
//        ret["title"] = Core::Variant ("jakis tytul");
//        return ret;
//}

/****************************************************************************/

void EditController::onOk (Ptr <Book> book)
{
#if 1
        std::cerr << "EditController::onOk (" << book->toString () << ")" << std::endl;
#endif
}

} /* namespace Catalog */
