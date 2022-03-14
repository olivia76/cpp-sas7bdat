/**
 *  \file filter.cpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#include "filter.hpp"
#include <fmt/core.h>

using namespace Rcpp;

cppsas7bdat::ColumnFilter::IncludeExclude Rcppsas7bdat::filter(SEXP _include,
                                                               SEXP _exclude) {
  auto list_to_set = [](std::string _context, auto &_set,
                        SEXP _object) -> void {
    if (Rf_isNull(_object))
      return;
    if (TYPEOF(_object) != STRSXP) {
      throw std::runtime_error(
          fmt::format("{} argument is not a vector of character", _context));
    }
    Rcpp::CharacterVector list(_object);
    const int n = list.size();
    for (int i = 0; i < n; ++i) {
      // std::cerr << "#" << i << '/' << n << ':' <<
      // Rcpp::as<std::string>(list[i]) << std::endl;
      _set.insert(Rcpp::as<std::string>(list[i]));
    }
  };

  cppsas7bdat::ColumnFilter::IncludeExclude f;
  list_to_set("include", f.included, _include);
  list_to_set("exclude", f.excluded, _exclude);
  return f;
}
