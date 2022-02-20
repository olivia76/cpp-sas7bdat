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
#include <iostream>

namespace cppsas7bdat {
  namespace INTERNAL {
    inline std::ifstream open_stream(const char* _pcszFileName)
    {
      std::cerr << "open_stream: [" << _pcszFileName << ']' << std::endl;
      std::ifstream is(_pcszFileName, std::ios::binary);
      if(!is.good()) EXCEPTION::not_a_valid_file(_pcszFileName);
      return is;
    }
  }
}

#endif
