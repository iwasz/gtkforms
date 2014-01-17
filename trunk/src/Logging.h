/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef LOGGING_H_
#define LOGGING_H_

#include <boost/move/utility.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
//#include <boost/log/utility/setup/file.hpp>
//#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_feature.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
//namespace keywords = boost::log::keywords;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT (logger, src::logger_mt)

#endif /* LOGGING_H_ */
