/**
 *  \file reader.cpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#include "reader.hpp"
#include "wrap_object.hpp"
#include <cppsas7bdat/datasource_ifstream.hpp>

using namespace Rcpp;

namespace {

  std::string to_string(const cppsas7bdat::DATETIME _x)
  {
    return boost::posix_time::to_simple_string(_x);
  }
  
  std::string to_string(const cppsas7bdat::DATE _x)
  {
    return boost::gregorian::to_simple_string(_x);
  }
  
  std::string to_string(const cppsas7bdat::TIME _x)
  {
    return boost::posix_time::to_simple_string(_x);
  }

}

namespace Rcpp {
  SEXP wrap_properties(const cppsas7bdat::Properties& _properties)
  {
    using namespace cppsas7bdat;
    
    auto header = Rcpp::List::create(Rcpp::Named("endianness") = std::string(to_string(_properties.header.endianness)),
				     Rcpp::Named("platform") = std::string(to_string(_properties.header.platform)),
				     Rcpp::Named("date_created") = to_string(_properties.header.date_created),
				     Rcpp::Named("date_modified") = to_string(_properties.header.date_modified),
				     Rcpp::Named("dataset_name") = _properties.header.dataset_name,
				     Rcpp::Named("encoding") = _properties.header.encoding,
				     Rcpp::Named("file_type") = _properties.header.file_type,
				     Rcpp::Named("sas_release") = _properties.header.sas_release,
				     Rcpp::Named("sas_server_type") = _properties.header.sas_server_type,
				     Rcpp::Named("os_type") = _properties.header.os_type,
				     Rcpp::Named("os_name") = _properties.header.os_name,
				     Rcpp::Named("header_length") = _properties.header.header_length,
				     Rcpp::Named("page_length") = _properties.header.page_length,
				     Rcpp::Named("page_count") = _properties.header.page_count
				     );
    
    auto columns = [&_properties]() {
		     const size_t ncols = _properties.metadata.columns.size();
		     auto columns = Rcpp::List(ncols);
		     auto names = Rcpp::StringVector(ncols);
		     for(size_t icol = 0; icol<ncols; ++icol) {
		       names[icol] = _properties.metadata.columns[icol].name;
		       columns[icol] = Rcpp::List::create(Rcpp::Named("name") = _properties.metadata.columns[icol].name,
							  Rcpp::Named("label") = _properties.metadata.columns[icol].label,
							  Rcpp::Named("format") = _properties.metadata.columns[icol].format,
							  Rcpp::Named("type") = std::string(to_string(_properties.metadata.columns[icol].type)));
		     }
		     columns.names() = names;
		     return columns;		     
		   }();
    
    auto metadata = Rcpp::List::create(Rcpp::Named("compression") = std::string(to_string(_properties.metadata.compression)),
				       Rcpp::Named("creator") = _properties.metadata.creator,
				       Rcpp::Named("creator_proc") = _properties.metadata.creator_proc,
				       Rcpp::Named("row_length") = _properties.metadata.row_length,
				       Rcpp::Named("row_count") = _properties.metadata.row_count,
				       Rcpp::Named("column_count") = _properties.metadata.column_count,
				       Rcpp::Named("col_count_p1") = _properties.metadata.col_count_p1,
				       Rcpp::Named("col_count_p2") = _properties.metadata.col_count_p2,
				       Rcpp::Named("mix_page_row_count") = _properties.metadata.mix_page_row_count,
				       Rcpp::Named("lcs") = _properties.metadata.lcs,
				       Rcpp::Named("lcp") = _properties.metadata.lcp,
				       Rcpp::Named("columns") = columns
				       );
    
    return Rcpp::List::create(Rcpp::Named("header") = header,
			      Rcpp::Named("metadata") = metadata);
  }
}

namespace {
  struct SinkBase {
    explicit SinkBase(Environment _sink)
      : sink(_sink),
	f_set_properties(sink["set_properties"])
    {
    }
    
    void set_properties([[maybe_unused]]const cppsas7bdat::Properties& _properties)
    {
      SEXP p = wrap_properties(_properties);
      f_set_properties(p);
    }
    
  protected:
    Environment sink;
    Function f_set_properties;
  };
  
  struct Sink : public SinkBase {
    explicit Sink(Environment _sink)
      : SinkBase(_sink),
	f_push_row(sink["push_row"])
    {
      //std::cerr << "Sink::Sink" << std::endl;
    }

    void set_properties([[maybe_unused]]const cppsas7bdat::Properties& _properties)
    {
      SinkBase::set_properties(_properties);
      columns = cppsas7bdat::COLUMNS(_properties.metadata.columns);
      setup_col_names();
    }
    
    void push_row([[maybe_unused]]const size_t _irow,
		  [[maybe_unused]]cppsas7bdat::Column::PBUF _p) {
      const size_t ncols = columns.size();
      for(size_t icol = 0; icol<ncols; ++icol) {
	const auto& column = columns[icol];
	switch(column.type) {
	case cppsas7bdat::Column::Type::string: row[icol] = std::string(column.get_string(_p)); break;
	case cppsas7bdat::Column::Type::integer: row[icol] = column.get_integer(_p); break;
	case cppsas7bdat::Column::Type::number: row[icol] = column.get_number(_p); break;
	case cppsas7bdat::Column::Type::datetime: row[icol] = to_string(column.get_datetime(_p)); break;
	case cppsas7bdat::Column::Type::date: row[icol] = to_string(column.get_date(_p)); break;
	case cppsas7bdat::Column::Type::time: row[icol] = to_string(column.get_time(_p)); break;
	}
      }
      f_push_row(_irow, row);
    }

    void end_of_data()
    {
    }

  protected:
    void setup_col_names()
    {
      const size_t ncols = columns.size();
      col_names = Rcpp::StringVector(ncols);
      for(size_t icol = 0; icol<ncols; ++icol) {
	col_names[icol] = columns[icol].name;
      }
      row = Rcpp::List(ncols);
      row.names() = col_names;
    }
    
    cppsas7bdat::COLUMNS columns;
    Function f_push_row;
    Rcpp::StringVector col_names;
    Rcpp::List row;
  };

  struct SinkChunk : public SinkBase {
    const size_t size;
    size_t idata{0};
    size_t istartrow{0}, iendrow{0}, ilastrow{0};

    using COL_NUMBERS = Rcpp::NumericVector;
    using COL_INTEGERS = Rcpp::IntegerVector;
    using COL_DATETIMES = Rcpp::StringVector;
    using COL_DATES = Rcpp::StringVector;
    using COL_TIMES = Rcpp::StringVector;
    using COL_STRINGS = Rcpp::StringVector;

    std::vector<COL_NUMBERS> col_numbers;
    std::vector<COL_INTEGERS> col_integers;
    std::vector<COL_DATETIMES> col_datetimes;
    std::vector<COL_DATES> col_dates;
    std::vector<COL_TIMES> col_times;
    std::vector<COL_STRINGS> col_strings;
    
    explicit SinkChunk(Environment _sink, const size_t _size)
      : SinkBase(_sink),
	f_push_rows(sink["push_rows"]),
	size(_size)
    {
      //std::cerr << "SinkChunk::SinkChunk(" << size << ')' << std::endl;
    }
    
    template<typename _Values>
    static void prepare_values(const cppsas7bdat::COLUMNS& _columns, _Values& _values, const size_t _size)
    {
      using COL = typename _Values::value_type;
      _values.resize(_columns.size(), COL(_size));
    }

    void prepare_values(const size_t _size)
    {
      prepare_values(columns.numbers, col_numbers, _size);
      prepare_values(columns.integers, col_integers, _size);
      prepare_values(columns.datetimes, col_datetimes, _size);
      prepare_values(columns.dates, col_dates, _size);
      prepare_values(columns.times, col_times, _size);
      prepare_values(columns.strings, col_strings, _size);
    }
    
    void set_properties([[maybe_unused]]const cppsas7bdat::Properties& _properties)
    {
      ilastrow = _properties.metadata.row_count;
      SinkBase::set_properties(_properties);
      columns = cppsas7bdat::Columns(_properties.metadata.columns);
      prepare_values(size);
      setup_col_names();
    }

    template<typename _Values, typename _Fct>
    void push_values(cppsas7bdat::Column::PBUF _p, const cppsas7bdat::COLUMNS& _columns, _Values& _values, _Fct fct)
    {
      const size_t ncols = _columns.size();
      for(size_t icol = 0; icol < ncols; ++icol) {
	_values[icol][idata] = fct(_columns[icol], _p);
      }
    }    
    
    void push_row([[maybe_unused]]const size_t _irow,
		  [[maybe_unused]]cppsas7bdat::Column::PBUF _p) {
      push_values(_p, columns.numbers, col_numbers, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return column.get_number(_p); });
      push_values(_p, columns.integers, col_integers, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return column.get_integer(_p); });
      push_values(_p, columns.datetimes, col_datetimes, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return to_string(column.get_datetime(_p)); });
      push_values(_p, columns.dates, col_dates, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return to_string(column.get_date(_p)); });
      push_values(_p, columns.times, col_times, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return to_string(column.get_time(_p)); });
      push_values(_p, columns.strings, col_strings, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return std::string(column.get_string(_p)); });
      
      iendrow = _irow;
      ++idata;
      if(idata == size) {
	flush();
	// Check if the were starting the last chunk => resize to a smaller chunk
	if(istartrow + size > ilastrow && istartrow < ilastrow) {
	  prepare_values(ilastrow-istartrow);
	}
      }
    }

    void end_of_data()
    {
      flush();
    }

    void flush()
    {
      size_t icol={0};
      auto set_values = [&](const auto& _values) {
			  for(const auto& v: _values) {
			    rows[icol++] = v;
			  }
			};
      
      set_values(col_numbers);
      set_values(col_integers);
      set_values(col_datetimes);
      set_values(col_dates);
      set_values(col_times);
      set_values(col_strings);

      f_push_rows(istartrow, iendrow, rows);
      idata = 0;
      istartrow = iendrow+1;
    }

  protected:
    void setup_col_names()
    {
      auto add_col_name = [&](const cppsas7bdat::COLUMNS& _columns) {
			    for(const auto& column: _columns) {
			      col_names.push_back(column.name);
			    }
			  };
      add_col_name(columns.numbers);
      add_col_name(columns.integers);
      add_col_name(columns.datetimes);
      add_col_name(columns.dates);
      add_col_name(columns.times);
      add_col_name(columns.strings);

      rows = Rcpp::List(col_names.size());
      rows.names() = col_names;
    }
    
    cppsas7bdat::Columns columns;  
    Function f_push_rows;  
    Rcpp::StringVector col_names;
    Rcpp::List rows;
  };
}

Rcppsas7bdat::Reader::Reader(std::string _inputfilename, SEXP _sink)
  : cppsas7bdat::Reader(_build(_inputfilename, _sink))
{
}

cppsas7bdat::Reader Rcppsas7bdat::Reader::_build(std::string _inputfilename, SEXP _sink)
{
  Environment sink(_sink);

  if(sink.exists("chunk_size") &&
     sink.exists("push_rows")) {
    const size_t chunk_size = sink["chunk_size"];
    //std::cerr << "Reader::_build::SinkChunk(" << chunk_size << ')' << std::endl;
    return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_inputfilename.c_str()), SinkChunk(sink, chunk_size));
  } else {
    //std::cerr << "Reader::_build::Sink()" << std::endl;
    return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_inputfilename.c_str()), Sink(sink));
  }
}

SEXP Rcppsas7bdat::Reader::build(std::string _inputfilename, SEXP _sink)
{
  return Rcpp::wrap_object(Reader(_inputfilename, _sink));
}

SEXP Rcppsas7bdat::Reader::properties() const
{
  return Rcpp::wrap_properties(cppsas7bdat::Reader::properties());
}

void Rcppsas7bdat::Reader::read_all()
{
  return cppsas7bdat::Reader::read_all();
}

bool Rcppsas7bdat::Reader::read_row()
{
  return cppsas7bdat::Reader::read_row();
}

bool Rcppsas7bdat::Reader::read_rows(const size_t _chunk_size)
{
  return cppsas7bdat::Reader::read_rows(_chunk_size);
}

size_t Rcppsas7bdat::Reader::current_row_index() const
{
  return cppsas7bdat::Reader::current_row_index();
}
