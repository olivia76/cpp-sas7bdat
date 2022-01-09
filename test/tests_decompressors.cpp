/**
 *  \file tests/tests_decompressors.cpp
 *
 *  \brief  
 *
 *  \author  Olivia Quinet
 */

#include <catch2/catch.hpp>
#include "../src/sas7bdat-impl.hpp"

using namespace cppsas7bdat;
using namespace cppsas7bdat::INTERNAL;
using namespace cppsas7bdat::INTERNAL::DECOMPRESSOR;

SCENARIO("When I pop values from a source, I get the expected values", "[internal][decompressor][SRC_VALUES][pop]")
{
  GIVEN("A source") {
    const unsigned char src1[] = {'a'};
    const BYTES source1{src1};
    auto src = SRC_VALUES(source1);
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
	CHECK(test == source1);
	CHECK(src.remaining() == 0);
      }
    }
  }
}

SCENARIO("The None decompressor just does a perfect forward of the data", "[internal][decompressor][None]")
{
  GIVEN("The none decompressor and a source") {
    const unsigned char src1[] = {'a'};
    const BYTES source1{src1};
    const auto decompressor = DECOMPRESSOR::None();
    WHEN("The source is passed to the decompressor") {
      auto test = decompressor(source1);
      THEN("I get the source back") {
	CHECK(test == source1);
      }
    }
  }
}

SCENARIO("I can store values in the destination", "[internal][decompressor][DST_VALUES]")
{
  GIVEN("A destination of one character") {
    const auto dst = DST_VALUES<Endian::little, Format::bit32>((size_t)1);
    WHEN("I check if one value can be added") {
      THEN("It is accepted") {
	CHECK(dst.check() == true);
	CHECK(dst.check(1) == true);
	CHECK_NOTHROW(dst.assert_check(1));
      }
    }
    WHEN("I check if more than one value can be added") {
      THEN("It is rejected") {
	CHECK(dst.check(2) == false);
	CHECK_THROWS(dst.assert_check(2));
      }
    }
  }
}
