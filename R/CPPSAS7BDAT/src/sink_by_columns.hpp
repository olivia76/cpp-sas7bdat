/**
 *  \file sink_by_columns.hpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_R_SINK_BY_COLUMNS_HPP_
#define _CPP_SAS7BDAT_R_SINK_BY_COLUMNS_HPP_

#include "sink_base.hpp"

namespace Rcppsas7bdat {

struct SinkByColumns : public SinkBase {
  const size_t size;
  size_t idata{0}, ilastrow{0};

  using COL_NUMBERS = Rcpp::NumericVector;
  using COL_INTEGERS = Rcpp::IntegerVector;
  using COL_DATETIMES = Rcpp::CharacterVector;
  using COL_DATES = Rcpp::CharacterVector;
  using COL_TIMES = Rcpp::CharacterVector;
  using COL_STRINGS = Rcpp::CharacterVector;

  COL_INTEGERS row_index;

  std::vector<COL_NUMBERS> col_numbers;
  std::vector<COL_INTEGERS> col_integers;
  std::vector<COL_DATETIMES> col_datetimes;
  std::vector<COL_DATES> col_dates;
  std::vector<COL_TIMES> col_times;
  std::vector<COL_STRINGS> col_strings;

  explicit SinkByColumns(Environment _sink, const size_t _size)
      : SinkBase(_sink), size(_size) {}

  template <typename _Values>
  static void prepare_values(const cppsas7bdat::COLUMNS &_columns,
                             _Values &_values, const size_t _size) {
    using COL = typename _Values::value_type;
    _values.resize(_columns.size());
    for (auto &val : _values)
      val = COL(_size);
  }

  void prepare_values(const size_t _size) {
    row_index = COL_INTEGERS(_size);

    prepare_values(columns.numbers, col_numbers, _size);
    prepare_values(columns.integers, col_integers, _size);
    prepare_values(columns.datetimes, col_datetimes, _size);
    prepare_values(columns.dates, col_dates, _size);
    prepare_values(columns.times, col_times, _size);
    prepare_values(columns.strings, col_strings, _size);
  }

  void set_properties([
      [maybe_unused]] const cppsas7bdat::Properties &_properties) {
    ilastrow = _properties /*.metadata*/.row_count;
    SinkBase::set_properties(_properties);
    columns = cppsas7bdat::Columns(_properties /*.metadata*/.columns);
    prepare_values(size ? std::min(size, ilastrow) : ilastrow);
    setup_col_names();
  }

  static void push_value(COL_STRINGS &_values, const size_t _idata,
                         const cppsas7bdat::SV _x) {
    _values[_idata] = std::string(_x);
  }

  static void push_value(COL_NUMBERS &_values, const size_t _idata,
                         const cppsas7bdat::NUMBER _x) {
    _values[_idata] = _x;
  }

  static void push_value(COL_INTEGERS &_values, const size_t _idata,
                         const cppsas7bdat::INTEGER _x) {
    _values[_idata] = _x;
  }

  static void push_value(COL_DATETIMES &_values, const size_t _idata,
                         const cppsas7bdat::DATETIME _x) {
    if (_x.is_not_a_date_time())
      _values[_idata] = NA_STRING; // R_NilValue;
    else
      _values[_idata] = cppsas7bdat::to_string(_x);
  }

  static void push_value(COL_DATES &_values, const size_t _idata,
                         const cppsas7bdat::DATE _x) {
    if (_x.is_not_a_date())
      _values[_idata] = NA_STRING; // R_NilValue;
    else
      _values[_idata] = cppsas7bdat::to_string(_x);
  }

  static void push_value(COL_TIMES &_values, const size_t _idata,
                         const cppsas7bdat::TIME _x) {
    if (_x.is_not_a_date_time())
      _values[_idata] = NA_STRING; // R_NilValue;
    else
      _values[_idata] = cppsas7bdat::to_string(_x);
  }

  template <typename _Values, typename _Fct>
  void push_values(cppsas7bdat::Column::PBUF _p,
                   const cppsas7bdat::COLUMNS &_columns, _Values &_values,
                   _Fct fct) {
    const size_t ncols = _columns.size();
    for (size_t icol = 0; icol < ncols; ++icol) {
      push_value(_values[icol], idata, fct(_columns[icol], _p));
    }
  }

  void push_row([[maybe_unused]] const size_t _irow,
                [[maybe_unused]] cppsas7bdat::Column::PBUF _p) {

    row_index[idata] = _irow;

    push_values(
        _p, columns.numbers, col_numbers,
        [](const cppsas7bdat::Column &column, cppsas7bdat::Column::PBUF _p) {
          return column.get_number(_p);
        });
    push_values(
        _p, columns.integers, col_integers,
        [](const cppsas7bdat::Column &column, cppsas7bdat::Column::PBUF _p) {
          return column.get_integer(_p);
        });
    push_values(
        _p, columns.datetimes, col_datetimes,
        [](const cppsas7bdat::Column &column, cppsas7bdat::Column::PBUF _p) {
          return column.get_datetime(_p);
        });
    push_values(
        _p, columns.dates, col_dates,
        [](const cppsas7bdat::Column &column, cppsas7bdat::Column::PBUF _p) {
          return column.get_date(_p);
        });
    push_values(
        _p, columns.times, col_times,
        [](const cppsas7bdat::Column &column, cppsas7bdat::Column::PBUF _p) {
          return column.get_time(_p);
        });
    push_values(
        _p, columns.strings, col_strings,
        [](const cppsas7bdat::Column &column, cppsas7bdat::Column::PBUF _p) {
          return column.get_string(_p);
        });

    ++idata;
  }

protected:
  template <typename _Vt> static _Vt resize_vector(const _Vt &_v, size_t _n) {
    _n = std::min((size_t)(_v.size()), _n);
    _Vt v(_n);
    for (size_t i = 0; i < _n; ++i)
      v[i] = _v[i];
    return v;
  }

  void set_values() {
    size_t icol = {0};

    auto set_values = [&](const auto &_columns) {
      for (const auto &column : _columns) {
        if (idata != column.size()) {
          rows[icol++] = resize_vector(column, idata);
        } else {
          rows[icol++] = column;
        }
      }
    };

    if (row_index.size() != idata)
      row_index = resize_vector(row_index, idata);
    set_values(col_numbers);
    set_values(col_integers);
    set_values(col_datetimes);
    set_values(col_dates);
    set_values(col_times);
    set_values(col_strings);
  }

  void setup_col_names() {
    auto add_col_name = [&](const cppsas7bdat::COLUMNS &_columns) {
      for (const auto &column : _columns) {
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
  Rcpp::StringVector col_names;
  Rcpp::List rows;
};

} // namespace Rcppsas7bdat

#endif
