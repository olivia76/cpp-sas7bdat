/**
 *  \file src/exceptions.hpp
 * 
 *  \brief List of exceptions
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_SRC_EXCEPTIONS_HPP_
#define _CPP_SAS7BDAT_SRC_EXCEPTIONS_HPP_

namespace cppsas7bdat {
  namespace EXCEPTION {
    void cannot_allocate_memory();
    void not_a_valid_file();
    void header_too_short();
    void invalid_magic_number();
    void cannot_read_page();
    void cannot_decompress();
  }
}

#endif
