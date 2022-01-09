/**
 *  \file tests/tests_decompressors.cpp
 *
 *  \brief  
 *
 *  \author  Olivia Quinet
 */

#include <catch2/catch.hpp>
#include "../src/sas7bdat-impl.hpp"

using namespace cppsas7bdat::INTERNAL;
using namespace cppsas7bdat::INTERNAL::DECOMPRESSOR;

SCENARIO("When I pop values from a source, I get the expected values", "[internal][decompressor][SRC_VALUES][pop")
{
  GIVEN("A source") {
    constexpr unsigned char s[] = {'a'};
    const BYTES source{s};
    auto src = SRC_VALUES(source);
    THEN("The remaining is one") {
      CHECK(src.check(1) == true);
      CHECK(src.remaining() == 1);
    }
    WHEN("I pop a value") {
      auto test = src.pop();
      THEN("I get then expected value") {
	CHECK(test == 'a');
	CHECK(src.remaining() == 0);
      }
    }
    WHEN("I pop all the values") {
      auto test = src.pop(1);
      THEN("I get then expected value") {
	CHECK(test == source);
	CHECK(src.remaining() == 0);
      }
    }
  }
}
