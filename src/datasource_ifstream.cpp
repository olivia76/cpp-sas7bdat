/**
 *  \file src/datasource_ifstream.cpp
 * 
 *  \brief DataSource ifstream
 *
 *  \author Olivia Quinet
 */

#include <cppsas7bdat/datasource_ifstream.hpp>
#include "exceptions.hpp"
#include "stream.hpp"

namespace cppsas7bdat::datasource {

  ifstream::ifstream(const char* _pcszfilename)
    : is(INTERNAL::open_stream(_pcszfilename))
  {
  }
}
