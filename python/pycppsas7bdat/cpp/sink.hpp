/**
 *  \file python/pycppsas7bdat/cpp/sink.hpp
 *
 *  \brief Python/C++ sinks
 *
 *  \author Olivia Quinet
 */

#ifndef _PYCPP_SAS7BDAT_SINK_HPP_
#define _PYCPP_SAS7BDAT_SINK_HPP_

#include <boost/python/list.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using namespace boost::python;

namespace pycppsas7bdat {

static PyObject *to_python(const cppsas7bdat::NUMBER _x) {
  return PyFloat_FromDouble(_x);
}
static PyObject *to_python(const cppsas7bdat::INTEGER _x) {
  return PyLong_FromLong(_x);
}
static PyObject *to_python(const boost::python::object &_o) {
  return boost::python::incref(_o.ptr());
}

struct SinkBase : public boost::noncopyable {
  // using STRING = boost::python::str;
  using STRING = boost::python::object;

  SinkBase(PyObject *_self) : self(_self) { Py_INCREF(self); }
  ~SinkBase() {
    if (self)
      Py_DECREF(self);
  }
  SinkBase(SinkBase &&_rhs) noexcept : self(_rhs.self) { _rhs.self = nullptr; }

  void set_cpp_attr() {
    // std::cerr << "SinkBase:: set_cpp_attr(" << this << ")" << std::endl;
    boost::python::object value(boost::ref(*this));
    PyObject *po = boost::python::incref(value.ptr());
    PyObject_SetAttrString(self, "_cpp", po);
  }

  void del_cpp_attr() { PyObject_DelAttrString(self, "_cpp"); }

  void set_properties([
      [maybe_unused]] const cppsas7bdat::Properties &_properties) {
    set_cpp_attr();
    call_method<void>(self, "set_properties",
                      boost::shared_ptr<cppsas7bdat::Properties>(
                          new cppsas7bdat::Properties(_properties)));
  }

  void flush_sink() {
    // std::cerr << "SinkBase::flush_sink(" << this << ")" << std::endl;
    end_of_data();
  }

  virtual void end_of_data() = 0;

  /*static auto to_str(const cppsas7bdat::SV& _x)
  {
    return STRING(_x.data(), _x.size());
    };*/

  static auto to_str(const cppsas7bdat::SV &_x) {
    auto pyObj = PyBytes_FromStringAndSize(_x.data(), _x.size());
    boost::python::handle<> handle(pyObj);
    return boost::python::object(handle);
  };

protected:
  PyObject *self;
};

struct Sink : public SinkBase {
  Sink(PyObject *_self) : SinkBase(_self) {}

  void set_properties([
      [maybe_unused]] const cppsas7bdat::Properties &_properties) {
    SinkBase::set_properties(_properties);
    columns = cppsas7bdat::COLUMNS(_properties /*.metadata*/.columns);
  }

  void push_row([[maybe_unused]] const size_t _irow,
                [[maybe_unused]] cppsas7bdat::Column::PBUF _p) {

    boost::python::list l;

    for (const auto &column : columns) {
      switch (column.type) {
      case cppsas7bdat::Column::Type::string:
        l.append(to_str(column.get_string(_p)));
        break;
      case cppsas7bdat::Column::Type::integer:
        l.append(column.get_integer(_p));
        break;
      case cppsas7bdat::Column::Type::number:
        l.append(column.get_number(_p));
        break;
      case cppsas7bdat::Column::Type::datetime:
        l.append(column.get_datetime(_p));
        break;
      case cppsas7bdat::Column::Type::date:
        l.append(column.get_date(_p));
        break;
      case cppsas7bdat::Column::Type::time:
        l.append(column.get_time(_p));
        break;
      }
    }

    call_method<void>(self, "push_row", _irow, l);
  }

  void end_of_data() override {
    del_cpp_attr();
    // std::cerr << "Sink::end_of_data()" << std::endl;
  }

protected:
  cppsas7bdat::COLUMNS columns;
};

struct SinkChunk : public SinkBase {
  size_t size;
  size_t idata{0};
  size_t istartrow{0}, iendrow{0};

  using COL_NUMBERS = std::vector<cppsas7bdat::NUMBER>;
  using COL_INTEGERS = std::vector<cppsas7bdat::INTEGER>;
  using COL_DATETIMES = std::vector<cppsas7bdat::DATETIME>;
  using COL_DATES = std::vector<cppsas7bdat::DATE>;
  using COL_TIMES = std::vector<cppsas7bdat::TIME>;
  using COL_STRINGS = std::vector<STRING>;

  /*class COL_STRINGS {
  private:
    size_t m_capacity{0};
    size_t m_size{0};
    size_t m_length{0};
    std::unique_ptr<char[]> m_data;

  public:
    using value_type = const char*;

    COL_STRINGS() = default;

    void* data() { return m_data.get(); }

    size_t capacity() const noexcept { return m_capacity; }
    size_t size() const noexcept { return m_size; }
    size_t length() const noexcept { return m_length; }

    void reserve(const size_t _size, const size_t _length)
    {
      m_capacity = _size;
      m_length = _length;
      m_data.reset(new char[capacity() * length()]);
      clear();
    }

    void emplace_back(const cppsas7bdat::SV& _sv)
    {
      std::memcpy(data() + m_size * length(), _sv.data(), std::min(_sv.size(),
  length())); m_size++;
    }

    void clear()
    {
      memset(data(), 0, capacity() * length());
      m_size = 0;
    }
    }; */

  std::vector<COL_NUMBERS> col_numbers;
  std::vector<COL_INTEGERS> col_integers;
  std::vector<COL_DATETIMES> col_datetimes;
  std::vector<COL_DATES> col_dates;
  std::vector<COL_TIMES> col_times;
  std::vector<COL_STRINGS> col_strings;

  SinkChunk(PyObject *_self, const size_t _size)
      : SinkBase(_self), size(_size) {}

  template <typename _Values>
  void prepare_values(const cppsas7bdat::COLUMNS &_columns, _Values &_values) {
    _values.resize(_columns.size());
    for (auto &v : _values)
      v.reserve(size);
  }

  template <typename _Values>
  void prepare_values_str(const cppsas7bdat::COLUMNS &_columns,
                          _Values &_values) {
    const size_t ncols = _columns.size();
    _values.resize(ncols);
    for (size_t icol = 0; icol < ncols; ++icol) {
      _values[icol].reserve(size, _columns[icol].length());
    }
  }

  void set_properties([
      [maybe_unused]] const cppsas7bdat::Properties &_properties) {
    SinkBase::set_properties(_properties);
    columns = cppsas7bdat::Columns(_properties /*.metadata*/.columns);
    prepare_values(columns.numbers, col_numbers);
    prepare_values(columns.integers, col_integers);
    prepare_values(columns.datetimes, col_datetimes);
    prepare_values(columns.dates, col_dates);
    prepare_values(columns.times, col_times);
    prepare_values(columns.strings, col_strings);
    // prepare_values_str(columns.strings, col_strings);
  }

  template <typename _Values, typename _Fct>
  static void push_values(cppsas7bdat::Column::PBUF _p,
                          const cppsas7bdat::COLUMNS &_columns,
                          _Values &_values, _Fct fct) {
    const size_t ncols = _columns.size();
    for (size_t icol = 0; icol < ncols; ++icol) {
      _values[icol].emplace_back(fct(_columns[icol], _p));
    }
  }

  void push_row([[maybe_unused]] const size_t _irow,
                [[maybe_unused]] cppsas7bdat::Column::PBUF _p) {
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
    // push_values(_p, columns.strings, col_strings, [](const
    // cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return
    // column.get_string(_p); });
    push_values(
        _p, columns.strings, col_strings,
        [](const cppsas7bdat::Column &column, cppsas7bdat::Column::PBUF _p) {
          return to_str(column.get_string(_p));
        });
    iendrow = _irow;
    ++idata;
    if (idata == size)
      flush();
  }

  void end_of_data() override {
    del_cpp_attr();
    // std::cerr << "SinkChunk::end_of_data()" << std::endl;
    flush();
  }

  template <typename _Values> static void clear_values(_Values &_values) {
    for (auto &v : _values)
      v.clear();
  }

  template <typename _Values>
  static auto to_nparray(_Values &_values /*, const int _type*/) {
    using T = typename _Values::value_type;
    namespace python = boost::python;
    namespace numpy = boost::python::numpy;
    return numpy::from_data(_values.data(), numpy::dtype::get_builtin<T>(),
                            python::make_tuple(_values.size()),
                            python::make_tuple(sizeof(T)), python::object());
    // npy_intp m[1] = { _values.size() };
    // python::object o(python::handle<>(PyArray_SimpleNewFromData(1, m, _type,
    // _values.data()))); PyObject* pyObj = PyArray_SimpleNewFromData(1, m,
    // _type, _values.data()); return boost::python::handle<(pyObj);
    // boost::python::handle<> handle( pyObj );
    // boost::python::numeric::array arr( handle );
    // return arr;
    // return o;
  }

  template <typename _Values>
  static auto to_nparray_obj(/*const size_t _size,*/ _Values &_values) {
    /*using T = typename _Values::value_type;
    namespace python   = boost::python;
    namespace numpy   = boost::python::numpy;
    return numpy::from_data(_values.data(),
                            numpy::dtype::get_builtin<char*>(),
                            python::make_tuple(_values.size()),
                            python::make_tuple(_size),
                            python::object());*/
    boost::python::list py_list;
    for (auto &s : _values) {
      py_list.append(s);
    }
    // return boost::python::numpy::array(py_list);
    return py_list;
    /*const size_t n = _values.size();
    auto* pyObj = PyList_New(n);
    Py_INCREF(pyObj);
    for(size_t i=0; i<n; ++i) {
      PyList_SetItem(pyObj, i, pycppsas7bdat::to_python(_values[i]));
    }
    boost::python::handle<> handle(pyObj);
    return boost::python::object(handle);*/
  }

  template <typename _Values, typename _Fct>
  static void set_dict_values(boost::python::dict &_d,
                              const cppsas7bdat::COLUMNS &_columns,
                              _Values &_values, _Fct _fct) {
    const size_t ncols = _columns.size();
    for (size_t icol = 0; icol < ncols; ++icol) {
      _d[_columns[icol].name] = _fct(_columns[icol], _values[icol]);
    }
  }

  void set_dict_values(boost::python::dict &_d) const {
    set_dict_values(
        _d, columns.numbers, col_numbers,
        []([[maybe_unused]] const cppsas7bdat::Column &_col, auto &_values) {
          return SinkChunk::to_nparray_obj(_values /*, NPY_DOUBLE*/);
        });
    set_dict_values(
        _d, columns.integers, col_integers,
        []([[maybe_unused]] const cppsas7bdat::Column &_col, auto &_values) {
          return SinkChunk::to_nparray_obj(_values /*, NPY_LONG*/);
        });
    set_dict_values(
        _d, columns.datetimes, col_datetimes,
        []([[maybe_unused]] const cppsas7bdat::Column &_col, auto &_values) {
          return SinkChunk::to_nparray_obj(_values /*, NPY_DATETIME*/);
        });
    set_dict_values(
        _d, columns.dates, col_dates,
        []([[maybe_unused]] const cppsas7bdat::Column &_col, auto &_values) {
          return SinkChunk::to_nparray_obj(_values /*, NPY_DATE*/);
        });
    set_dict_values(
        _d, columns.times, col_times,
        []([[maybe_unused]] const cppsas7bdat::Column &_col, auto &_values) {
          return SinkChunk::to_nparray_obj(_values /*, NPY_TIME*/);
        });
    set_dict_values(
        _d, columns.strings, col_strings,
        []([[maybe_unused]] const cppsas7bdat::Column &_col, auto &_values) {
          return SinkChunk::to_nparray_obj(
              /*_col.length(),*/ _values);
        });
  }

  virtual void flush() {
    if (idata) {
      boost::python::dict d;
      set_dict_values(d);
      call_method<void>(self, "push_rows", istartrow, iendrow, d);
      clear_values();
    }
  }

  void clear_values() {
    idata = 0;
    istartrow = iendrow + 1;
    clear_values(col_numbers);
    clear_values(col_integers);
    clear_values(col_datetimes);
    clear_values(col_dates);
    clear_values(col_times);
    clear_values(col_strings);
  }

protected:
  cppsas7bdat::Columns columns;
};

class SinkData : public SinkChunk {
public:
  SinkData(PyObject *_self) : SinkChunk(_self, 0) {}

  void set_properties([
      [maybe_unused]] const cppsas7bdat::Properties &_properties) {
    size = _properties /*.metadata*/.row_count;
    SinkChunk::set_properties(_properties);
  }

  void flush() override {}

  void end_of_data() override {
    del_cpp_attr();
    // std::cerr << "SinkData::end_of_data()" << std::endl;
    if (idata) {
      boost::python::dict d;
      set_dict_values(d);
      call_method<void>(self, "set_data", d);
      clear_values();
    }
  }
};

} // namespace pycppsas7bdat

#endif
