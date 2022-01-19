/**
 *  \file python/pycppsas7bdat/cpp/pycppsas7bdat.cpp
 * 
 *  \brief Main file for generating the python module
 *
 *  \author Olivia Quinet
 */

#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/numpy.hpp>
//#include <numpy/ndarrayobject.h>
#include <cppsas7bdat/version.hpp>
#include <cppsas7bdat/sas7bdat.hpp>
#include <cppsas7bdat/datasource_ifstream.hpp>
#include "import_datetime.hpp"

using namespace boost::python;

namespace {

  struct SinkBase {
    using STRING = boost::python::str;
    
    SinkBase(PyObject* _self)
      : self(_self)
    {
      Py_INCREF(self);
    }
    ~SinkBase()
    {
      if(self) Py_DECREF(self);
    }
    SinkBase(SinkBase&& _rhs) noexcept
      : self(_rhs.self)
    {
      _rhs.self = nullptr;
    }

    void set_properties([[maybe_unused]]const cppsas7bdat::Properties& _properties)
    {
      call_method<void>(self, "set_properties", boost::ref(_properties));
    }

    void end_of_data() const noexcept {}
    
    static auto to_str(const cppsas7bdat::SV& _x)
    {
      return STRING(_x.data(), _x.size());
    };

  protected:
    PyObject* self;
  };

  struct Sink : public SinkBase {
    Sink(PyObject* _self) : SinkBase(_self) {}

    void set_properties([[maybe_unused]]const cppsas7bdat::Properties& _properties)
    {
      SinkBase::set_properties(_properties);
      columns = cppsas7bdat::COLUMNS(_properties.metadata.columns);
    }
    
    void push_row([[maybe_unused]]const size_t _irow,
		  [[maybe_unused]]cppsas7bdat::Column::PBUF _p) {
      
      boost::python::list l;
      
      for(const auto& column: columns) {
	switch(column.type) {
	case cppsas7bdat::Column::Type::string: l.append(to_str(column.get_string(_p))); break;
	case cppsas7bdat::Column::Type::integer: l.append(column.get_integer(_p)); break;
	case cppsas7bdat::Column::Type::number: l.append(column.get_number(_p)); break;
	case cppsas7bdat::Column::Type::datetime: l.append(column.get_datetime(_p)); break;
	case cppsas7bdat::Column::Type::date: l.append(column.get_date(_p)); break;
	case cppsas7bdat::Column::Type::time: l.append(column.get_time(_p)); break;
	}
      }
      
      call_method<void>(self, "push_row", _irow, l);
    }

  protected:
    cppsas7bdat::COLUMNS columns;
  };

  struct SinkChunk : public SinkBase {
    const size_t size;
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
	std::memcpy(data() + m_size * length(), _sv.data(), std::min(_sv.size(), length()));	
	m_size++;
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
        
    SinkChunk(PyObject* _self, const size_t _size) : SinkBase(_self), size(_size) {}

    template<typename _Values>
    void prepare_values(const cppsas7bdat::COLUMNS& _columns, _Values& _values)
    {
      _values.resize(_columns.size());
      for(auto& v: _values) v.reserve(size);
    }

    template<typename _Values>
    void prepare_values_str(const cppsas7bdat::COLUMNS& _columns, _Values& _values)
    {
      const size_t ncols = _columns.size();
      _values.resize(ncols);
      for(size_t icol=0; icol < ncols; ++icol) {
	_values[icol].reserve(size, _columns[icol].length());
      }
    }
    
    void set_properties([[maybe_unused]]const cppsas7bdat::Properties& _properties)
    {
      SinkBase::set_properties(_properties);
      columns = cppsas7bdat::Columns(_properties.metadata.columns);
      prepare_values(columns.numbers, col_numbers);
      prepare_values(columns.integers, col_integers);
      prepare_values(columns.datetimes, col_datetimes);
      prepare_values(columns.dates, col_dates);
      prepare_values(columns.times, col_times);
      prepare_values(columns.strings, col_strings);
      //prepare_values_str(columns.strings, col_strings);
    }

    template<typename _Values, typename _Fct>
    static void push_values(cppsas7bdat::Column::PBUF _p, const cppsas7bdat::COLUMNS& _columns, _Values& _values, _Fct fct)
    {
      const size_t ncols = _columns.size();
      for(size_t icol = 0; icol < ncols; ++icol) {
	_values[icol].emplace_back(fct(_columns[icol], _p));
      }
    }
    
    void push_row([[maybe_unused]]const size_t _irow,
		  [[maybe_unused]]cppsas7bdat::Column::PBUF _p) {
      push_values(_p, columns.numbers, col_numbers, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return column.get_number(_p); });
      push_values(_p, columns.integers, col_integers, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return column.get_integer(_p); });
      push_values(_p, columns.datetimes, col_datetimes, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return column.get_datetime(_p); });
      push_values(_p, columns.dates, col_dates, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return column.get_date(_p); });
      push_values(_p, columns.times, col_times, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return column.get_time(_p); });
      //push_values(_p, columns.strings, col_strings, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return column.get_string(_p); });
      push_values(_p, columns.strings, col_strings, [](const cppsas7bdat::Column& column, cppsas7bdat::Column::PBUF _p) { return to_str(column.get_string(_p)); });
      iendrow = _irow;
      ++idata;
      if(idata == size) flush();
    }

    void end_of_data()
    {
      flush();
    }

    template<typename _Values>
    static void flush_values(_Values& _values)
    {
      for(auto& v: _values) v.clear();
    }

    template<typename _Values>
    static auto to_nparray(_Values& _values/*, const int _type*/) {
      using T = typename _Values::value_type;
      namespace python   = boost::python;
      namespace numpy   = boost::python::numpy;
      return numpy::from_data(_values.data(),
			      numpy::dtype::get_builtin<T>(),
			      python::make_tuple(_values.size()),
			      python::make_tuple(sizeof(T)),
			      python::object());
      //npy_intp m[1] = { _values.size() };
      //python::object o(python::handle<>(PyArray_SimpleNewFromData(1, m, _type, _values.data())));
      //PyObject* pyObj = PyArray_SimpleNewFromData(1, m, _type, _values.data());
      //return boost::python::handle<(pyObj);
      //boost::python::handle<> handle( pyObj );
      //boost::python::numeric::array arr( handle );
      //return arr;
      //return o;
    }

    template<typename _Values>
    static auto to_nparray_obj(/*const size_t _size,*/_Values& _values) {
      /*using T = typename _Values::value_type;
      namespace python   = boost::python;
      namespace numpy   = boost::python::numpy;
      return numpy::from_data(_values.data(),
			      numpy::dtype::get_builtin<char*>(),
			      python::make_tuple(_values.size()),
			      python::make_tuple(_size),
			      python::object());*/
      boost::python::list py_list;
      for(auto& s: _values){
        py_list.append(s);
      }
      //return boost::python::numpy::array(py_list);
      return py_list;
    }

    template<typename _Values, typename _Fct>
    static void set_dict_values(boost::python::dict& _d, const cppsas7bdat::COLUMNS& _columns, _Values& _values, _Fct _fct)
    {
      const size_t ncols = _columns.size();
      for(size_t icol = 0; icol < ncols; ++icol) {
	_d[_columns[icol].name] = _fct(_columns[icol], _values[icol]);
      }
    }

    void flush()
    {
      boost::python::dict d;

      set_dict_values(d, columns.numbers, col_numbers, [](const cppsas7bdat::Column& _col, auto& _values) { return SinkChunk::to_nparray_obj(_values/*, NPY_DOUBLE*/); });
      set_dict_values(d, columns.integers, col_integers, [](const cppsas7bdat::Column& _col, auto& _values) { return SinkChunk::to_nparray_obj(_values/*, NPY_LONG*/); });
      set_dict_values(d, columns.datetimes, col_datetimes, [](const cppsas7bdat::Column& _col, auto& _values) { return SinkChunk::to_nparray_obj(_values/*, NPY_DATETIME*/); });
      set_dict_values(d, columns.dates, col_dates, [](const cppsas7bdat::Column& _col, auto& _values) { return SinkChunk::to_nparray_obj(_values/*, NPY_DATE*/); });
      set_dict_values(d, columns.times, col_times, [](const cppsas7bdat::Column& _col, auto& _values) { return SinkChunk::to_nparray_obj(_values/*, NPY_TIME*/); });
      set_dict_values(d, columns.strings, col_strings, [](const cppsas7bdat::Column& _col, auto& _values) { return SinkChunk::to_nparray_obj(/*_col.length(),*/_values); });
      
      call_method<void>(self, "push_rows", istartrow, iendrow, d);
      idata = 0;
      istartrow = iendrow+1;
      flush_values(col_numbers);
      flush_values(col_integers);
      flush_values(col_datetimes);
      flush_values(col_dates);
      flush_values(col_times);
      flush_values(col_strings);
    }

  protected:
    cppsas7bdat::Columns columns;
  };

  struct Reader : public cppsas7bdat::Reader {
    Reader(const char* _filename, PyObject* _sink)
      : cppsas7bdat::Reader(build(_filename, _sink))
    {
    }

    static cppsas7bdat::Reader build(const char* _filename, PyObject* _sink)
    {
      object sink(handle<>(borrowed(_sink)));

      if(PyObject_HasAttrString(_sink, "chunk") &&
	 PyObject_HasAttrString(_sink, "push_rows") ) {
	const size_t size = extract<size_t>(sink.attr("chunk"));
	return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_filename),
				   SinkChunk(_sink, size));
      } else {
	return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_filename),
				   Sink(_sink));
      }
    }

  };
}

/*auto init_numpy()
{
  import_array();
  }*/

BOOST_PYTHON_MODULE(cpp)  //_pycppsas7bdat)
{
  /*if(PyArray_API == NULL) {
    init_numpy(); 
    }*/
  using namespace boost::python;
  Py_Initialize();
  numpy::initialize();

  pycppsas7bdat::bind_datetime();
  
  def("getVersion", cppsas7bdat::getVersion);

  enum_<cppsas7bdat::Endian>("Endian")
    .value("unknown", cppsas7bdat::Endian::unknown)
    .value("little", cppsas7bdat::Endian::little)
    .value("big", cppsas7bdat::Endian::big)
    ;
  enum_<cppsas7bdat::Format>("Format")
    .value("bit32", cppsas7bdat::Format::bit32)
    .value("bit64", cppsas7bdat::Format::bit64)
    ;
  enum_<cppsas7bdat::Platform>("Platform")
    .value("unknown", cppsas7bdat::Platform::unknown)
    .value("unix", cppsas7bdat::Platform::unix)
    .value("windows", cppsas7bdat::Platform::windows)
    ;
  enum_<cppsas7bdat::Compression>("Compression")
    .value("none", cppsas7bdat::Compression::none)
    .value("RLE", cppsas7bdat::Compression::RLE)
    .value("RDC", cppsas7bdat::Compression::RDC)
    ;
  enum_<cppsas7bdat::Column::Type>("ColumnType")
    .value("unknown", cppsas7bdat::Column::Type::unknown)
    .value("string", cppsas7bdat::Column::Type::string)
    .value("number", cppsas7bdat::Column::Type::number)
    .value("integer", cppsas7bdat::Column::Type::integer)
    .value("datetime", cppsas7bdat::Column::Type::datetime)
    .value("date", cppsas7bdat::Column::Type::date)
    .value("time", cppsas7bdat::Column::Type::time)
    ;
  class_< cppsas7bdat::Properties::Header, boost::noncopyable >("Header", no_init )
    .def_readonly("format", &cppsas7bdat::Properties::Header::format)
    .def_readonly("endianness", &cppsas7bdat::Properties::Header::endianness)
    .def_readonly("platform", &cppsas7bdat::Properties::Header::platform)
    .def_readonly("date_created", &cppsas7bdat::Properties::Header::date_created)
    .def_readonly("date_modified", &cppsas7bdat::Properties::Header::date_modified)
    .def_readonly("dataset_name", &cppsas7bdat::Properties::Header::dataset_name)
    .def_readonly("encoding", &cppsas7bdat::Properties::Header::encoding)
    .def_readonly("file_type", &cppsas7bdat::Properties::Header::file_type)
    .def_readonly("sas_release", &cppsas7bdat::Properties::Header::sas_release)
    .def_readonly("sas_server_type", &cppsas7bdat::Properties::Header::sas_server_type)
    .def_readonly("os_type", &cppsas7bdat::Properties::Header::os_type)
    .def_readonly("os_name", &cppsas7bdat::Properties::Header::os_name)
    .def_readonly("header_length", &cppsas7bdat::Properties::Header::header_length)
    .def_readonly("page_length", &cppsas7bdat::Properties::Header::page_length)
    .def_readonly("page_count", &cppsas7bdat::Properties::Header::page_count)
    ;
  class_< cppsas7bdat::Properties::Metadata, boost::noncopyable >("Metadata", no_init )
    .def_readonly("compression", &cppsas7bdat::Properties::Metadata::compression)
    .def_readonly("creator", &cppsas7bdat::Properties::Metadata::creator)
    .def_readonly("creator_proc", &cppsas7bdat::Properties::Metadata::creator_proc)
    .def_readonly("row_length", &cppsas7bdat::Properties::Metadata::row_length)
    .def_readonly("row_count", &cppsas7bdat::Properties::Metadata::row_count)
    .def_readonly("column_count", &cppsas7bdat::Properties::Metadata::column_count)
    .def_readonly("col_count_p1", &cppsas7bdat::Properties::Metadata::col_count_p1)
    .def_readonly("col_count_p2", &cppsas7bdat::Properties::Metadata::col_count_p2)
    .def_readonly("mix_page_row_count", &cppsas7bdat::Properties::Metadata::mix_page_row_count)
    .def_readonly("lcs", &cppsas7bdat::Properties::Metadata::lcs)
    .def_readonly("lcp", &cppsas7bdat::Properties::Metadata::lcp)
    .def_readonly("columns", &cppsas7bdat::Properties::Metadata::columns)
    ;
  class_< cppsas7bdat::Column, boost::noncopyable >("Column", no_init )
    .def_readonly("name", &cppsas7bdat::Column::name)
    .def_readonly("format", &cppsas7bdat::Column::format)
    .def_readonly("label", &cppsas7bdat::Column::label)
    .def_readonly("type", &cppsas7bdat::Column::type)
    ;
  class_< std::vector< cppsas7bdat::Column, std::allocator<cppsas7bdat::Column> >, boost::noncopyable >("Columns", no_init )
    .def("__iter__", iterator< std::vector< cppsas7bdat::Column >, return_internal_reference<> >() )
    ;
  class_< cppsas7bdat::Properties, boost::noncopyable >("Properties", no_init)
    .def_readonly("header", &cppsas7bdat::Properties::header)
    .def_readonly("metadata", &cppsas7bdat::Properties::metadata)
    ;
  //class_< Sink, boost::noncopyable >("Sink", init<PyObject*>() )
  //  .def("set_properties", &Sink::set_properties)
  //  .def("push_row", &Sink::push_row)
  //  ;
  class_< Reader, boost::noncopyable >("Reader", init<const char*, PyObject*>() )
    .add_property("current_row_index", &Reader::current_row_index)
    .def("read_all", &Reader::read_all)
    .def("read_row", &Reader::read_row)
    .def("read_rows", &Reader::read_rows)
    ;
}
