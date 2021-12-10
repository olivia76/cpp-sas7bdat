/**
 *  \file cppsas7bdat/reader.hpp
 * 
 *  \brief Reader
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_READER_HPP_
#define _CPP_SAS7BDAT_READER_HPP_

#include <cppsas7bdat/column.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>

namespace cppsas7bdat {
  namespace reader {
    struct print {
      COLUMNS columns;
      
      void set_properties(const Properties& _properties) {
	columns = COLUMNS(_properties.metadata.columns);
	fmt::print("#");
	for(const auto& column: columns) {
	  fmt::print(",");
	  fmt::print(column.name);
	}
	fmt::print("\n");
      }
      
      void read_row(const size_t _irow, Column::PBUF _p) {
	fmt::print("{}", _irow);
	for(const auto& column: columns) {
	  fmt::print(",");
	  switch(column.type()) {
	  case cppsas7bdat::Column::Type::string: fmt::print("{}", column.get_string(_p)); break;
	  case cppsas7bdat::Column::Type::integer: fmt::print("{}", column.get_integer(_p)); break;
	  case cppsas7bdat::Column::Type::number: fmt::print("{}", column.get_number(_p)); break;
	  case cppsas7bdat::Column::Type::datetime: fmt::print("{}", column.get_datetime(_p)); break;
	  case cppsas7bdat::Column::Type::date: fmt::print("{}", column.get_date(_p)); break;
	  case cppsas7bdat::Column::Type::time: fmt::print("{}", column.get_time(_p)); break;
	  }
	}
	fmt::print("\n");
      }
    };
  }  
}

#endif
