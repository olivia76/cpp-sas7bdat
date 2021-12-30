/**
 *  \file tests/tests_interface.cpp
 *
 *  \brief  
 *
 *  \author  Olivia Quinet
 */

#include <catch2/catch.hpp>
#include "../include/cppsas7bdat/sas7bdat.hpp"
#include "../include/cppsas7bdat/datasource_ifstream.hpp"
#include "../include/cppsas7bdat/datasink_null.hpp"
#include "data.hpp"

namespace {
  auto get_reader(const char* _pcszfilename) {
    return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_pcszfilename), cppsas7bdat::datasink::null());
  }  
}

SCENARIO("When I try to read a non existing file with the public interface, an exception is thrown", "[interface][not_a_valid_file]")
{
  GIVEN("An invalid path") {
    THEN("An exception is thrown") {
      REQUIRE_THROWS_WITH(get_reader(invalid_path), "not_a_valid_file");
    }
  }
}


SCENARIO("When I try to read a file too short with the public interface, an exception is thrown", "[interface][file too short]")
{
  GIVEN("A path to a too short file") {
    THEN("an exception is thrown") {
      REQUIRE_THROWS_WITH(get_reader(file_too_short), "header_too_short");
    }
  }
}

SCENARIO("When I try to read a file with an invalid magic number with the public interface, an exception is thrown", "[interface][header_too_short]")
{
  GIVEN("A path to a file with an invalid magic number") {
    THEN("an exception is thrown") {
      REQUIRE_THROWS_WITH(get_reader(invalid_magic_number), "invalid_magic_number");
    }
  }
}

SCENARIO("When I try to read a valid file with the public interface, no exception is thrown", "[interface][valid_file]")
{
  GIVEN("A path to a valid file") {
    THEN("No exception is thrown") {
      REQUIRE_NOTHROW(get_reader(file1));
    }
  }
}
