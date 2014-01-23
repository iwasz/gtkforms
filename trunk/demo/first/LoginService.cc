/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "LoginService.h"

bool LoginService::checkLogin (std::string const &login, std::string const &password)
{
        return (login == "iwasz" && password == "iwasz");
}
