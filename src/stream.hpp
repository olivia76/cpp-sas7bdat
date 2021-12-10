/**
 *  \file src/stream.hpp
 * 
 *  \brief Stream management
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_SRC_STREAM_HPP_
#define _CPP_SAS7BDAT_SRC_STREAM_HPP_

#include <fstream>

namespace cppsas7bdat {
  namespace INTERNAL {
    inline std::ifstream open_stream(const char* _pcszFileName)
    {
      std::ifstream is(_pcszFileName, std::ios::binary);
      if(!is) EXCEPTION::not_a_valid_file();
      return is;
    }
  }
}

#endif
