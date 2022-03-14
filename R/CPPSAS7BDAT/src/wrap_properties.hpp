/**
 *  \file wrap_properties.hpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_R_WRAP_PROPERTIES_HPP_
#define _CPP_SAS7BDAT_R_WRAP_PROPERTIES_HPP_

#include "wrap_object.hpp"
#include <Rcpp.h>
#include <cppsas7bdat/properties.hpp>

namespace Rcppsas7bdat {
using namespace Rcpp;

class ListBuilder {
public:
  ListBuilder() = default;
  ~ListBuilder() = default;
  ListBuilder(ListBuilder const &) = delete;

  inline ListBuilder &add(const std::string &name, SEXP x) {
    names.push_back(name);
    elements.push_back(PROTECT(x));
    return *this;
  }

  template <typename T>
  inline ListBuilder &add(const std::string &name, const T &x) {
    names.push_back(name);
    elements.push_back(PROTECT(wrap(x)));
    return *this;
  }

  List to_list() const {
    List result(elements.size());
    for (size_t i = 0; i < elements.size(); ++i) {
      result[i] = elements[i];
    }
    result.attr("names") = wrap(names);
    UNPROTECT(elements.size());
    return result;
  }

  inline operator List() const { return to_list(); }

private:
  std::vector<std::string> names;
  std::vector<SEXP> elements;
};
} // namespace Rcppsas7bdat

namespace Rcpp {
SEXP wrap_properties(const cppsas7bdat::Properties &_properties) {
  using namespace cppsas7bdat;

  auto columns = [&_properties]() {
    const size_t ncols = _properties /*.metadata*/.columns.size();
    auto columns = Rcpp::List(ncols);
    auto names = Rcpp::StringVector(ncols);
    for (size_t icol = 0; icol < ncols; ++icol) {
      names[icol] = _properties /*.metadata*/.columns[icol].name;
      columns[icol] = Rcpp::List::create(
          Rcpp::Named("name") = _properties /*.metadata*/.columns[icol].name,
          Rcpp::Named("label") = _properties /*.metadata*/.columns[icol].label,
          Rcpp::Named("format") =
              _properties /*.metadata*/.columns[icol].format,
          Rcpp::Named("type") = std::string(
              to_string(_properties /*.metadata*/.columns[icol].type)));
    }
    columns.names() = names;
    return columns;
  }();

  return Rcppsas7bdat::ListBuilder()
      .add("endianness",
           std::string(to_string(_properties /*.header*/.endianness)))
      .add("platform", std::string(to_string(_properties /*.header*/.platform)))
      .add("date_created", to_string(_properties /*.header*/.date_created))
      .add("date_modified", to_string(_properties /*.header*/.date_modified))
      .add("dataset_name", _properties /*.header*/.dataset_name)
      .add("encoding", _properties /*.header*/.encoding)
      .add("file_type", _properties /*.header*/.file_type)
      .add("sas_release", _properties /*.header*/.sas_release)
      .add("sas_server_type", _properties /*.header*/.sas_server_type)
      .add("os_type", _properties /*.header*/.os_type)
      .add("os_name", _properties /*.header*/.os_name)
      .add("header_length", _properties /*.header*/.header_length)
      .add("page_length", _properties /*.header*/.page_length)
      .add("page_count", _properties /*.header*/.page_count)
      .add("compression",
           std::string(to_string(_properties /*.metadata*/.compression)))
      .add("creator", _properties /*.metadata*/.creator)
      .add("creator_proc", _properties /*.metadata*/.creator_proc)
      .add("row_length", _properties /*.metadata*/.row_length)
      .add("row_count", _properties /*.metadata*/.row_count)
      .add("column_count", _properties /*.metadata*/.column_count)
      .add("col_count_p1", _properties /*.metadata*/.col_count_p1)
      .add("col_count_p2", _properties /*.metadata*/.col_count_p2)
      .add("mix_page_row_count", _properties /*.metadata*/.mix_page_row_count)
      .add("lcs", _properties /*.metadata*/.lcs)
      .add("lcp", _properties /*.metadata*/.lcp)
      .add("columns", columns)
      .to_list();
}
} // namespace Rcpp

#endif
