/**
 *  \file python/pycppsas7bdat/cpp/reader.cpp
 *
 *  \brief Python/C++ reader
 *
 *  \author Olivia Quinet
 */

// clang-format off
#include <cppsas7bdat/filter/column.hpp>
#include <cppsas7bdat/source/ifstream.hpp>
#include <cppsas7bdat/reader.hpp>
#include <boost/python.hpp>
#include "reader.hpp"
#include "sink.hpp"
#include <fmt/core.h>
// clang-format on

namespace pycppsas7bdat {
struct Reader : public cppsas7bdat::Reader {
  Reader(const char *_filename, PyObject *_sink, PyObject *_include,
         PyObject *_exclude)
      : cppsas7bdat::Reader(build(_filename, _sink, _include, _exclude)) {}

  static cppsas7bdat::ColumnFilter::IncludeExclude filter(PyObject *_include,
                                                          PyObject *_exclude) {
    auto list_to_set = [](std::string _context, auto &_set,
                          PyObject *_object) -> void {
      if (!_object || _object == Py_None)
        return;
      if (!PyList_Check(_object)) {
        throw std::runtime_error(
            fmt::format("{} argument is not a list", _context));
      }
      const int n = PyList_Size(_object);
      for (int i = 0; i < n; ++i) {
        auto item = PyList_GetItem(_object, i);
        try {
          _set.insert(boost::python::extract<std::string>(item));
        } catch (...) {
          throw std::runtime_error(
              fmt::format("Element of {} argument is not a string", _context));
        }
      }
    };

    cppsas7bdat::ColumnFilter::IncludeExclude f;
    list_to_set("include", f.included, _include);
    list_to_set("exclude", f.excluded, _exclude);
    return f;
  }

  static cppsas7bdat::Reader build(const char *_filename, PyObject *_sink,
                                   PyObject *_include, PyObject *_exclude) {
    using namespace boost::python;
    object sink(handle<>(borrowed(_sink)));

    if (PyObject_HasAttrString(_sink, "chunk_size") &&
        PyObject_HasAttrString(_sink, "set_properties") &&
        PyObject_HasAttrString(_sink, "push_rows")) {
      const size_t chunk_size = extract<size_t>(sink.attr("chunk_size"));
      return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_filename),
                                 SinkChunk(_sink, chunk_size),
                                 filter(_include, _exclude));
    } else if (PyObject_HasAttrString(_sink, "set_properties") &&
               PyObject_HasAttrString(_sink, "set_data")) {
      return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_filename),
                                 SinkData(_sink), filter(_include, _exclude));
    } else if (PyObject_HasAttrString(_sink, "set_properties") &&
               PyObject_HasAttrString(_sink, "push_row")) {
      return cppsas7bdat::Reader(cppsas7bdat::datasource::ifstream(_filename),
                                 Sink(_sink), filter(_include, _exclude));
    } else {
      throw std::runtime_error("Not a valid sink");
    }
  }
};

boost::shared_ptr<Reader> create_reader(std::string _filename, PyObject *_sink,
                                        PyObject *_include = nullptr,
                                        PyObject *_exclude = nullptr) {
  return boost::shared_ptr<Reader>(
      new Reader(_filename.c_str(), _sink, _include, _exclude));
}

void bind_reader() {
  using namespace boost::python;

  class_<SinkBase, boost::noncopyable>("SinkBase", no_init)
      .def("flush_sink", &SinkBase::flush_sink);

  class_<Reader, boost::noncopyable>("Reader", no_init)
      .def("__init__", make_constructor(&create_reader, default_call_policies(),
                                        (arg("filename"), arg("sink"),
                                         arg("include") = object(),
                                         arg("exclude") = object())))
      .add_property("current_row_index", &Reader::current_row_index)
      .def("properties", &Reader::properties,
           return_value_policy<reference_existing_object>())
      .def("read_all", &Reader::read_all)
      .def("read_row", &Reader::read_row)
      .def("read_rows", &Reader::read_rows)
      .def("skip", &Reader::skip)
      .def("end_of_data", &Reader::end_of_data);
}
} // namespace pycppsas7bdat
