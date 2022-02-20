/**
 *  \file src/stream.cpp
 * 
 *  \brief Stream
 *
 *  \author Olivia Quinet
 */

#include "stream.hpp"
#include "exceptions.hpp"
#include <spdlog/spdlog.h>
#include <fmt/core.h>

std::ifstream cppsas7bdat::INTERNAL::open_stream(const char* _pcszFileName)
{
  spdlog::critical(fmt::format("open_stream: [{}]", _pcszFileName));
  std::ifstream is(_pcszFileName, std::ios::binary);
  if(!is.good()) EXCEPTION::not_a_valid_file(_pcszFileName);
  else {
    spdlog::critical(fmt::format("open_stream: [{}] is valid", _pcszFileName));
  }
  return is;
}
