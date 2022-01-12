/**
 *  \file python/main.cpp
 * 
 *  \brief Main file for generating the python module
 *
 *  \author Olivia Quinet
 */

#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <cppsas7bdat/version.hpp>
#include <cppsas7bdat/sas7bdat.hpp>
#include <cppsas7bdat/datasource_ifstream.hpp>

using namespace boost::python;

namespace {

  struct Sink {
    Sink(PyObject* _self)
      : self(_self)
    {
      Py_INCREF(self);
    }
    ~Sink()
    {
      if(self) Py_DECREF(self);
    }
    Sink(Sink&& _rhs) noexcept
      : self(_rhs.self),
	columns(std::move(_rhs.columns))
    {
      _rhs.self = nullptr;
    }

    void set_properties([[maybe_unused]]const cppsas7bdat::Properties& _properties)
    {
      columns = cppsas7bdat::COLUMNS(_properties.metadata.columns);
      call_method<void>(self, "set_properties", boost::ref(_properties));
    }
      
    void push_row([[maybe_unused]]const size_t _irow,
		  [[maybe_unused]]cppsas7bdat::Column::PBUF _p) {

      auto to_str = [](const cppsas7bdat::SV& _x) {
		      return str(_x.data(), _x.size());
		    };
      
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

  private:
    PyObject* self;
    cppsas7bdat::COLUMNS columns;
  };

  struct Reader : public cppsas7bdat::Reader {
    Reader(const char* _filename, PyObject* _sink)
      : cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_filename),
			    Sink(_sink))
    {
    }

  };
  
}

BOOST_PYTHON_MODULE(pycppsas7bdat)
{
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
  class_< Sink, boost::noncopyable >("Sink", init<PyObject*>() )
    .def("set_properties", &Sink::set_properties)
    .def("push_row", &Sink::push_row)
    ;
  class_< Reader, boost::noncopyable >("Reader", init<const char*, PyObject*>() )
    .add_property("current_row_index", &Reader::current_row_index)
    .def("read_all", &Reader::read_all)
    .def("read_row", &Reader::read_row)
    .def("read_rows", &Reader::read_rows)
    ;
}
