/**
 *  \file tests/tests_exceptions.cpp
 *
 *  \brief  
 *
 *  \author  Olivia Quinet
 */

#include <catch2/catch.hpp>
#include "../src/exceptions.hpp"

SCENARIO("I can throw the different exceptions")
{
  CHECK_THROWS(cppsas7bdat::EXCEPTION::cannot_allocate_memory());
  CHECK_THROWS(cppsas7bdat::EXCEPTION::not_a_valid_file());
  CHECK_THROWS(cppsas7bdat::EXCEPTION::header_too_short());
  CHECK_THROWS(cppsas7bdat::EXCEPTION::invalid_magic_number());
  CHECK_THROWS(cppsas7bdat::EXCEPTION::cannot_read_page());
  CHECK_THROWS(cppsas7bdat::EXCEPTION::cannot_decompress());
  CHECK_THROWS(cppsas7bdat::EXCEPTION::invalid_buffer_access(0, 0, 0));
}
