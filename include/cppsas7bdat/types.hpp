/**
 *  \file cppsas7bdat/types.hpp
 * 
 *  \brief Types description
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_TYPES_HPP_
#define _CPP_SAS7BDAT_TYPES_HPP_

#include <string>
#include <cstdint>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>

namespace cppsas7bdat {

  using STRING = std::string;
  using SV = std::string_view;
  using NUMBER = double;
  using INTEGER = int32_t;
  using DATETIME = boost::posix_time::ptime;
  using DATE = boost::gregorian::date;
  using TIME = boost::posix_time::time_duration;

  std::string to_string(DATETIME);
  std::string to_string(DATE);
  std::string to_string(TIME);
}

#endif
