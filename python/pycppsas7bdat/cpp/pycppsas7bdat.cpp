/**
 *  \file python/pycppsas7bdat/cpp/pycppsas7bdat.cpp
 *
 *  \brief Main file for generating the python module
 *
 *  \author Olivia Quinet
 */

// clang-format off
#include <boost/python.hpp>
#include <cppsas7bdat/reader.hpp>
#include <cppsas7bdat/version.hpp>
#include "import_datetime.hpp"
#include "reader.hpp"
// clang-format on

namespace pycppsas7bdat {
inline PyObject *to_python(const std::string &x) {
  return PyUnicode_FromStringAndSize(x.c_str(), x.length());
}
} // namespace pycppsas7bdat

BOOST_PYTHON_MODULE(cpp) {
  using namespace boost::python;
  Py_Initialize();

  pycppsas7bdat::bind_datetime();
  pycppsas7bdat::bind_reader();

  def("getVersion", cppsas7bdat::getVersion);

  enum_<cppsas7bdat::Endian>("Endian")
      .value("unknown", cppsas7bdat::Endian::unknown)
      .value("little", cppsas7bdat::Endian::little)
      .value("big", cppsas7bdat::Endian::big);
  enum_<cppsas7bdat::Format>("Format")
      .value("bit32", cppsas7bdat::Format::bit32)
      .value("bit64", cppsas7bdat::Format::bit64);
  enum_<cppsas7bdat::Platform>("Platform")
      .value("unknown", cppsas7bdat::Platform::unknown)
      .value("unix", cppsas7bdat::Platform::unix)
      .value("windows", cppsas7bdat::Platform::windows);
  enum_<cppsas7bdat::Compression>("Compression")
      .value("none", cppsas7bdat::Compression::none)
      .value("RLE", cppsas7bdat::Compression::RLE)
      .value("RDC", cppsas7bdat::Compression::RDC);
  enum_<cppsas7bdat::Column::Type>("ColumnType")
      .value("unknown", cppsas7bdat::Column::Type::unknown)
      .value("string", cppsas7bdat::Column::Type::string)
      .value("number", cppsas7bdat::Column::Type::number)
      .value("integer", cppsas7bdat::Column::Type::integer)
      .value("datetime", cppsas7bdat::Column::Type::datetime)
      .value("date", cppsas7bdat::Column::Type::date)
      .value("time", cppsas7bdat::Column::Type::time);
  class_<cppsas7bdat::Column, boost::noncopyable>("Column", no_init)
      .def_readonly("name", &cppsas7bdat::Column::name)
      .def_readonly("format", &cppsas7bdat::Column::format)
      .def_readonly("label", &cppsas7bdat::Column::label)
      .def_readonly("type", &cppsas7bdat::Column::type);
  class_<std::vector<cppsas7bdat::Column, std::allocator<cppsas7bdat::Column>>,
         boost::noncopyable>("Columns", no_init)
      .def("__iter__", iterator<std::vector<cppsas7bdat::Column>,
                                return_internal_reference<>>());

  struct properties {
    static auto get_date_created(const cppsas7bdat::Properties &p) {
      return pycppsas7bdat::to_python(p.date_created);
    }
    static auto get_date_modified(const cppsas7bdat::Properties &p) {
      return pycppsas7bdat::to_python(p.date_modified);
    }
  };

  class_<cppsas7bdat::Properties, std::shared_ptr<cppsas7bdat::Properties>,
         boost::noncopyable>("Properties", no_init)
      .def_readonly("format", &cppsas7bdat::Properties /*::Header*/ ::format)
      .def_readonly("endianness",
                    &cppsas7bdat::Properties /*::Header*/ ::endianness)
      .def_readonly("platform",
                    &cppsas7bdat::Properties /*::Header*/ ::platform)
      .add_property("date_created", &properties::get_date_created)
      .add_property("date_modified", &properties::get_date_modified)
      .def_readonly("dataset_name",
                    &cppsas7bdat::Properties /*::Header*/ ::dataset_name)
      .def_readonly("encoding",
                    &cppsas7bdat::Properties /*::Header*/ ::encoding)
      .def_readonly("file_type",
                    &cppsas7bdat::Properties /*::Header*/ ::file_type)
      .def_readonly("sas_release",
                    &cppsas7bdat::Properties /*::Header*/ ::sas_release)
      .def_readonly("sas_server_type",
                    &cppsas7bdat::Properties /*::Header*/ ::sas_server_type)
      .def_readonly("os_type", &cppsas7bdat::Properties /*::Header*/ ::os_type)
      .def_readonly("os_name", &cppsas7bdat::Properties /*::Header*/ ::os_name)
      .def_readonly("header_length",
                    &cppsas7bdat::Properties /*::Header*/ ::header_length)
      .def_readonly("page_length",
                    &cppsas7bdat::Properties /*::Header*/ ::page_length)
      .def_readonly("page_count",
                    &cppsas7bdat::Properties /*::Header*/ ::page_count)
      .def_readonly("compression",
                    &cppsas7bdat::Properties /*::Metadata*/ ::compression)
      .def_readonly("creator",
                    &cppsas7bdat::Properties /*::Metadata*/ ::creator)
      .def_readonly("creator_proc",
                    &cppsas7bdat::Properties /*::Metadata*/ ::creator_proc)
      .def_readonly("row_length",
                    &cppsas7bdat::Properties /*::Metadata*/ ::row_length)
      .def_readonly("row_count",
                    &cppsas7bdat::Properties /*::Metadata*/ ::row_count)
      .def_readonly("column_count",
                    &cppsas7bdat::Properties /*::Metadata*/ ::column_count)
      .def_readonly("col_count_p1",
                    &cppsas7bdat::Properties /*::Metadata*/ ::col_count_p1)
      .def_readonly("col_count_p2",
                    &cppsas7bdat::Properties /*::Metadata*/ ::col_count_p2)
      .def_readonly(
          "mix_page_row_count",
          &cppsas7bdat::Properties /*::Metadata*/ ::mix_page_row_count)
      .def_readonly("lcs", &cppsas7bdat::Properties /*::Metadata*/ ::lcs)
      .def_readonly("lcp", &cppsas7bdat::Properties /*::Metadata*/ ::lcp)
      .def_readonly("columns",
                    &cppsas7bdat::Properties /*::Metadata*/ ::columns);

  register_ptr_to_python<boost::shared_ptr<cppsas7bdat::Properties>>();
}
