/**
 *  \file src/types.cpp
 * 
 *  \brief Types file
 *
 *  \author Olivia Quinet
 */

#include <cppsas7bdat/types.hpp>

namespace cppsas7bdat {
  
  std::string to_string(DATETIME _x)
  {
    char szBuffer[32];
    const auto d = _x.date();
    const auto t = _x.time_of_day();
    if(t.fractional_seconds()) {
      std::sprintf(szBuffer, "%04d-%02d-%02d %02d:%02d:%02d.%06d", (int)d.year(), (int)d.month(), (int)d.day(), (int)t.hours(), (int)t.minutes(), (int)t.seconds(), (int)t.fractional_seconds());
    } else {
      std::sprintf(szBuffer, "%04d-%02d-%02d %02d:%02d:%02d", (int)d.year(), (int)d.month(), (int)d.day(), (int)t.hours(), (int)t.minutes(), (int)t.seconds());
    }
    return szBuffer;
  }
  
  std::string to_string(DATE _x)
  {
    char szBuffer[16];
    std::sprintf(szBuffer, "%04d-%02d-%02d", (int)_x.year(), (int)_x.month(), (int)_x.day());
    return szBuffer;
  }
  
  std::string to_string(TIME _x)
  {
    char szBuffer[16];
    if(_x.fractional_seconds()) {
      std::sprintf(szBuffer, "%02d:%02d:%02d.%06d", (int)_x.hours(), (int)_x.minutes(), (int)_x.seconds(), (int)_x.fractional_seconds());
    } else {
      std::sprintf(szBuffer, "%02d:%02d:%02d", (int)_x.hours(), (int)_x.minutes(), (int)_x.seconds());
    }
    return szBuffer;
  }

}
