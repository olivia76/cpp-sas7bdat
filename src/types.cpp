/**
 *  \file src/types.cpp
 *
 *  \brief Types file
 *
 *  \author Olivia Quinet
 */

#include <cppsas7bdat/types.hpp>
#include <fmt/core.h>

namespace cppsas7bdat {

std::string to_string(DATETIME _x) {
  char szBuffer[32];
  size_t size{0};
  const auto d = _x.date();
  const auto t = _x.time_of_day();
  if (t.fractional_seconds()) {
    auto r = fmt::format_to_n(szBuffer, sizeof(szBuffer),
                              "{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:06}",
                              d.year(), d.month(), d.day(), t.hours(),
                              t.minutes(), t.seconds(), t.fractional_seconds());
    size = r.size;
  } else {
    auto r = fmt::format_to_n(
        szBuffer, sizeof(szBuffer), "{:04}-{:02}-{:02} {:02}:{:02}:{:02}",
        d.year(), d.month(), d.day(), t.hours(), t.minutes(), t.seconds());
    size = r.size;
  }
  return std::string(szBuffer, size);
}

std::string to_string(DATE _x) {
  char szBuffer[16];
  size_t size{0};
  auto r = fmt::format_to_n(szBuffer, sizeof(szBuffer), "{:04}-{:02}-{:02}",
                            _x.year(), _x.month(), _x.day());
  size = r.size;
  return std::string(szBuffer, size);
}

std::string to_string(TIME _x) {
  char szBuffer[16];
  size_t size{0};
  if (_x.fractional_seconds()) {
    auto r = fmt::format_to_n(
        szBuffer, sizeof(szBuffer), "{:02}:{:02}:{:02}.{:06}", _x.hours(),
        _x.minutes(), _x.seconds(), _x.fractional_seconds());
    size = r.size;
  } else {
    auto r = fmt::format_to_n(szBuffer, sizeof(szBuffer), "{:02}:{:02}:{:02}",
                              _x.hours(), _x.minutes(), _x.seconds());
    size = r.size;
  }
  return std::string(szBuffer, size);
}

} // namespace cppsas7bdat
