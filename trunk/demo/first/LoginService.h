/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef LOGINSERVICE_H_
#define LOGINSERVICE_H_

#include <string>

class LoginService {
public:

        bool checkLogin (std::string const &login, std::string const &password);

};

#endif /* LOGINSERVICE_H_ */
