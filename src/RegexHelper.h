/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef REGEXHELPER_H_
#define REGEXHELPER_H_

namespace GtkForms {

struct RegexHelper {

        static bool inputNameMatches (std::string const &widgetName, std::string const &dataRange);
        static bool nameMatches (std::string const &modelName, std::string const &dataRange);
};

} /* namespace GtkForms */

#endif /* REGEXHELPER_H_ */
