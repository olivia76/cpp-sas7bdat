/**
 *  \file reader.hpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_R_WRAP_OBJECT_HPP_
#define _CPP_SAS7BDAT_R_WRAP_OBJECT_HPP_

#include <Rcpp.h>

namespace Rcpp {
/// wrap a C++ object for R use
/**
 *  \tparam _Tp  Type of the C++ object
 *  \param obj   A move reference to the C++ object
 *  \return      A valid SEXP reference containing a pointer to the C++ object
 */
template <typename _Tp>
inline SEXP
wrap_object(_Tp &&obj) { // inspired from "make_new_object" from Rcpp/Module.h
  Rcpp::XPtr<_Tp> xp(new _Tp(std::move(obj)),
                     true); // copy and mark as finalizable
  Rcpp::Function maker =
      Rcpp::Environment::Rcpp_namespace()["cpp_object_maker"];
  SEXP r = maker(typeid(_Tp).name(), xp);
  // PROTECT(r);
  return r;
}

} // namespace Rcpp

#endif
