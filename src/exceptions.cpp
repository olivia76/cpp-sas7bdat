/**
 *  \file src/exceptions.cpp
 * 
 *  \brief Exceptions
 *
 *  \author Olivia Quinet
 */

#include "exceptions.hpp"
#include <stdexcept>
#include <fmt/core.h>

namespace cppsas7bdat {
  void EXCEPTION::cannot_allocate_memory()
  {
    fmt::print(stderr, "cannot_allocate_memory\n");
    throw std::runtime_error("cannot_allocate_memory");
  }
  
  void EXCEPTION::not_a_valid_file()
  {
    fmt::print(stderr, "not_a_valid_file\n");
    throw std::runtime_error("not_a_valid_file");
  }
  
  void EXCEPTION::header_too_short()
  {
    fmt::print(stderr, "header_too_short\n");
    throw std::runtime_error("header_too_short");
  }
  
  void EXCEPTION::invalid_magic_number()
  {
    fmt::print(stderr, "invalid_magic_number\n");
    throw std::runtime_error("invalid_magic_number");
  }

  void EXCEPTION::cannot_read_page()
  {
    fmt::print(stderr, "cannot_read_page\n");
    throw std::runtime_error("cannot_read_page");
  }
}
