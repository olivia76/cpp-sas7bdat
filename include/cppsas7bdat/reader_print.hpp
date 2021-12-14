/**
 *  \file cppsas7bdat/reader_print.hpp
 * 
 *  \brief Simple reader that print out the content of the file
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_READER_PRINT_HPP_
#define _CPP_SAS7BDAT_READER_PRINT_HPP_

#include <cppsas7bdat/column.hpp>
#include <fmt/core.h>
#include <fmt/ostream.h>

namespace cppsas7bdat {
  namespace reader {
    struct print {
      std::ostream& os;
      
      print(std::ostream& _os)
	: os(_os)
      {
      }
      
      COLUMNS columns;

      void print_properties(const Properties& _properties) {
	fmt::print(os, "Properties:\n");
	fmt::print(os, "  format: {}\n", _properties.header.format);
	fmt::print(os, "  endianness: {}\n", _properties.header.endianness);
	fmt::print(os, "  platform: {}\n", _properties.header.platform);
	fmt::print(os, "  date created: {}\n", _properties.header.date_created);
	fmt::print(os, "  date modified: {}\n", _properties.header.date_modified);
	fmt::print(os, "  dataset name: {}\n", _properties.header.dataset_name);
	fmt::print(os, "  encoding: {}\n", _properties.header.encoding);
	fmt::print(os, "  file type: {}\n", _properties.header.file_type);
	fmt::print(os, "  SAS release: {}\n", _properties.header.sas_release);
	fmt::print(os, "  SAS server type: {}\n", _properties.header.sas_server_type);
	fmt::print(os, "  OS type: {}\n", _properties.header.os_type);
	fmt::print(os, "  OS name: {}\n", _properties.header.os_name);
	fmt::print(os, "  compression: {}\n", _properties.metadata.compression);
	fmt::print(os, "  creator: {}\n", _properties.metadata.creator);
	fmt::print(os, "  creator proc: {}\n", _properties.metadata.creator_proc);
	fmt::print(os, "  row count: {}\n", _properties.metadata.row_count);
	fmt::print(os, "  column count: {}\n", _properties.metadata.column_count);
	fmt::print(os, "Columns:\n");
	size_t icol=0;
	for(const auto& column: _properties.metadata.columns) {
	  fmt::print(os, "  - column #{}\n", icol);
	  fmt::print(os, "    name: {}\n", column.name);
	  fmt::print(os, "    label: {}\n", column.label);
	  fmt::print(os, "    format: {}\n", column.format);
	  fmt::print(os, "    type: {}\n", column.type());
	  ++icol;
	}
	fmt::print(os, "Data:\n");
      }
      
      void set_properties(const Properties& _properties) {
	print_properties(_properties);
	columns = COLUMNS(_properties.metadata.columns);
	fmt::print(os, "#");
	for(const auto& column: columns) {
	  fmt::print(os, ",");
	  fmt::print(os, column.name);
	}
	fmt::print(os, "\n");
      }
      
      void read_row(const size_t _irow, Column::PBUF _p) {
	fmt::print(os, "{}", _irow);
	for(const auto& column: columns) {
	  fmt::print(os, ",");
	  switch(column.type()) {
	  case cppsas7bdat::Column::Type::string: fmt::print(os, "{}", column.get_string(_p)); break;
	  case cppsas7bdat::Column::Type::integer: fmt::print(os, "{}", column.get_integer(_p)); break;
	  case cppsas7bdat::Column::Type::number: fmt::print(os, "{}", column.get_number(_p)); break;
	  case cppsas7bdat::Column::Type::datetime: fmt::print(os, "{}", column.get_datetime(_p)); break;
	  case cppsas7bdat::Column::Type::date: fmt::print(os, "{}", column.get_date(_p)); break;
	  case cppsas7bdat::Column::Type::time: fmt::print(os, "{}", column.get_time(_p)); break;
	  }
	}
	fmt::print(os, "\n");
      }
    };
  }  
}

#endif
