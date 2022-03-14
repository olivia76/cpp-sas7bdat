/**
 *  \file sink.hpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_R_SINK_HPP_
#define _CPP_SAS7BDAT_R_SINK_HPP_

#include "sink_base.hpp"

namespace Rcppsas7bdat {
struct Sink : public SinkBase {
  explicit Sink(Environment _sink)
      : SinkBase(_sink), f_push_row(sink["push_row"]) {}

  void set_properties([
      [maybe_unused]] const cppsas7bdat::Properties &_properties) {
    SinkBase::set_properties(_properties);
    columns = cppsas7bdat::COLUMNS(_properties /*.metadata*/.columns);
    setup_col_names();
  }

  void process_value(const size_t icol, const cppsas7bdat::SV _x) {
    row[icol] = std::string(_x);
  }

  void process_value(const size_t icol, const cppsas7bdat::INTEGER _x) {
    row[icol] = _x;
  }

  void process_value(const size_t icol, const cppsas7bdat::NUMBER _x) {
    if (std::isnan(_x))
      row[icol] = NA_REAL;
    else
      row[icol] = _x;
  }

  void process_value(const size_t icol, const cppsas7bdat::DATETIME _x) {
    if (_x.is_not_a_date_time())
      row[icol] = R_NilValue;
    else
      row[icol] = cppsas7bdat::to_string(_x);
  }

  void process_value(const size_t icol, const cppsas7bdat::DATE _x) {
    if (_x.is_not_a_date())
      row[icol] = R_NilValue;
    else
      row[icol] = cppsas7bdat::to_string(_x);
  }

  void process_value(const size_t icol, const cppsas7bdat::TIME _x) {
    if (_x.is_not_a_date_time())
      row[icol] = R_NilValue;
    else
      row[icol] = cppsas7bdat::to_string(_x);
  }

  void push_row([[maybe_unused]] const size_t _irow,
                [[maybe_unused]] cppsas7bdat::Column::PBUF _p) {
    const size_t ncols = columns.size();
    for (size_t icol = 0; icol < ncols; ++icol) {
      const auto &column = columns[icol];
      switch (column.type) {
      case cppsas7bdat::Column::Type::string:
        process_value(icol, column.get_string(_p));
        break;
      case cppsas7bdat::Column::Type::integer:
        process_value(icol, column.get_integer(_p));
        break;
      case cppsas7bdat::Column::Type::number:
        process_value(icol, column.get_number(_p));
        break;
      case cppsas7bdat::Column::Type::datetime:
        process_value(icol, column.get_datetime(_p));
        break;
      case cppsas7bdat::Column::Type::date:
        process_value(icol, column.get_date(_p));
        break;
      case cppsas7bdat::Column::Type::time:
        process_value(icol, column.get_time(_p));
        break;
      }
    }
    f_push_row(_irow, row);
  }

  void end_of_data() {}

protected:
  void setup_col_names() {
    const size_t ncols = columns.size();
    col_names = Rcpp::StringVector(ncols);
    for (size_t icol = 0; icol < ncols; ++icol) {
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

} // namespace Rcppsas7bdat

#endif
