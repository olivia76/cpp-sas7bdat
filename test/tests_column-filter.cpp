/**
 *  \file tests/tests_column-filter.cpp
 *
 *  \brief  
 *
 *  \author  Olivia Quinet
 */

#include <catch2/catch.hpp>
#include <cppsas7bdat/column-filter.hpp>
#include <cppsas7bdat/sas7bdat.hpp>
#include <cppsas7bdat/datasource_ifstream.hpp>
#include <cppsas7bdat/datasink_null.hpp>
#include "data.hpp"

SCENARIO("The IncludeExclude column filter can be used to filter columns")
{
  GIVEN("An include filter") {
    cppsas7bdat::ColumnFilter::IncludeExclude filter{{"col1"}, {}};
    WHEN("A filter a column based on its name") {
      CHECK(filter.is_accepted("col1") == true);
      CHECK(filter.is_accepted("col2") == false);
    }
  }
  GIVEN("An exclude filter") {
    cppsas7bdat::ColumnFilter::IncludeExclude filter{{}, {"col1"}};
    WHEN("A filter a column based on its name") {
      CHECK(filter.is_accepted("col1") == false);
      CHECK(filter.is_accepted("col2") == true);
    }
  }
}

namespace {
  template<typename _DataSink, typename _Filter>
  auto get_reader(const std::string& _filename, _DataSink&& _datasink, _Filter&& _filter) {
    return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(convert_path(_filename).c_str()), std::forward<_DataSink>(_datasink), std::forward<_Filter>(_filter));
  }  
}

SCENARIO("The IncludeExclude column filter can be used to filter columns to be read")
{
  const std::string filename="data/file2.sas7bdat";
  GIVEN(fmt::format("A file {},", filename)) {
    GIVEN("An include filter") {
      cppsas7bdat::ColumnFilter::IncludeExclude filter{{"c1"}, {}};
      auto reader = get_reader(filename, cppsas7bdat::datasink::null(), filter);
      const auto& columns = reader.properties().metadata.columns;
      THEN("The columns list is consistent with the filter") {
	REQUIRE(columns.size() == 1);
	CHECK(columns[0].name == "c1");
      }
    }
    GIVEN("An exclude filter") {
      cppsas7bdat::ColumnFilter::IncludeExclude filter{{}, {"c1"}};
      auto reader = get_reader(filename, cppsas7bdat::datasink::null(), filter);
      const auto& columns = reader.properties().metadata.columns;
      THEN("The columns list is consistent with the filter") {
	REQUIRE(columns.size() == 3);
	CHECK(columns[0].name == "q1");
	CHECK(columns[1].name == "c2");
	CHECK(columns[2].name == "q2");

      }
    }
  }
}
