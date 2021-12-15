/**
 *  \file src/exceptions.cpp
 * 
 *  \brief Exceptions
 *
 *  \author Olivia Quinet
 */

#include "exceptions.hpp"
#include <stdexcept>
#include <spdlog/spdlog.h>

namespace cppsas7bdat {
  void raise_exception(const char* _msg)
  {
    spdlog::critical(_msg);
    throw std::runtime_error(_msg);
  }
  
  void EXCEPTION::cannot_allocate_memory()
  {
    raise_exception("cannot_allocate_memory\n");
  }
  
  void EXCEPTION::not_a_valid_file()
  {
    raise_exception("not_a_valid_file\n");
  }
  
  void EXCEPTION::header_too_short()
  {
    raise_exception("header_too_short\n");
  }
  
  void EXCEPTION::invalid_magic_number()
  {
    raise_exception("invalid_magic_number\n");
  }

  void EXCEPTION::cannot_read_page()
  {
    raise_exception("cannot_read_page\n");
  }

  void EXCEPTION::cannot_decompress()
  {
    raise_exception("cannot_decompress\n");
  }
}
