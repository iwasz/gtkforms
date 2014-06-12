/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <boost/regex.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/replace.hpp>
#include "RegexHelper.h"

namespace GtkForms {

/*--------------------------------------------------------------------------*/

bool RegexHelper::inputNameMatches (std::string const &widgetName, std::string *inputName, std::string const &dataRange, bool outputs)
{
#if 0
        BOOST_LOG (lg) << widgetName;
#endif

        boost::regex e;

        if (dataRange.empty ()) {
                if (outputs) {
                        e = boost::regex {"[!>](.*)"};
                }
                else {
                        e = boost::regex {"[!<](.*)"};
                }
        }
        else {
                std::string copy = dataRange;
                boost::replace_all (copy, ".", "\\.*");
                boost::replace_all (copy, "*", ".*");

                if (outputs) {
                        e = boost::regex {"[!>](" + copy + ")"};
                }
                else {
                        e = boost::regex {"[!<](" + copy + ")"};
                }
        }

        boost::smatch what;

        if (boost::regex_match (widgetName, what, e, boost::match_extra)) {
                if (what.size () == 2) {
                        *inputName = what[1];
                }

                return true;
        }

        return false;
}

/****************************************************************************/

bool RegexHelper::modelNameMatches (std::string const &modelName, std::string const &dataRange)
{
        boost::regex e;

        if (dataRange.empty ()) {
                return true;
        }
        else {
                std::string copy = dataRange;
                boost::replace_all (copy, ".", "\\.*");
                boost::replace_all (copy, "*", ".*");
                e = boost::regex {copy};
        }

        boost::smatch what;

        if (boost::regex_match (modelName, what, e, boost::match_extra)) {
                return true;
        }

        return false;
}

} /* namespace GtkForms */
