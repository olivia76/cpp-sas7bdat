/**
 *  \file python/pycppsas7bdat/cpp/reader.cpp
 * 
 *  \brief Python/C++ reader
 *
 *  \author Olivia Quinet
 */

#include <boost/python.hpp>
#include <cppsas7bdat/datasource_ifstream.hpp>
#include <cppsas7bdat/sas7bdat.hpp>
#include "reader.hpp"
#include "sink.hpp"

namespace pycppsas7bdat {
  struct Reader : public cppsas7bdat::Reader {
    Reader(const char* _filename, PyObject* _sink)
      : cppsas7bdat::Reader(build(_filename, _sink))
    {
    }

    static cppsas7bdat::Reader build(const char* _filename, PyObject* _sink)
    {
      using namespace boost::python;
      object sink(handle<>(borrowed(_sink)));

      if(PyObject_HasAttrString(_sink, "chunk_size") &&
	 PyObject_HasAttrString(_sink, "set_properties") &&
	 PyObject_HasAttrString(_sink, "push_rows") ) {
	const size_t chunk_size = extract<size_t>(sink.attr("chunk_size"));
	return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_filename),
				   SinkChunk(_sink, chunk_size));
      } else if(PyObject_HasAttrString(_sink, "set_properties") &&
		PyObject_HasAttrString(_sink, "set_data")) {
	return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_filename),
				   SinkData(_sink));
      } else if(PyObject_HasAttrString(_sink, "set_properties") &&
		PyObject_HasAttrString(_sink, "push_row")){
	return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_filename),
				   Sink(_sink));
      } else {
	throw std::runtime_error("Not a valid sink");
      }
    }

  };

  void bind_reader()
  {
    using namespace boost::python;
      
    class_< Reader, boost::noncopyable >("Reader", init<const char*, PyObject*>() )
      .add_property("current_row_index", &Reader::current_row_index)
      .def("read_all", &Reader::read_all)
      .def("read_row", &Reader::read_row)
      .def("read_rows", &Reader::read_rows)
      ;
  }
}
  
