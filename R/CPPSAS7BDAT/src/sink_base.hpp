/**
 *  \file sink_base.hpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_R_SINK_BASE_HPP_
#define _CPP_SAS7BDAT_R_SINK_BASE_HPP_

#include "wrap_properties.hpp"
#include <Rcpp.h>

namespace Rcppsas7bdat {
using namespace Rcpp;

struct SinkBase {
  explicit SinkBase(Environment _sink)
      : sink(_sink), f_set_properties(sink["set_properties"]) {}

  void set_properties([
      [maybe_unused]] const cppsas7bdat::Properties &_properties) {
    SEXP p = wrap_properties(_properties);
    f_set_properties(p);
  }

protected:
  Environment sink;
  Function f_set_properties;
};

} // namespace Rcppsas7bdat

#endif
