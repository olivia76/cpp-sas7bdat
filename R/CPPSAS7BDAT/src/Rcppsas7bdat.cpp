/**
 *  \file Rcppsas7bdat.cpp
 *
 *  \brief  Definition of the Rcppsas7bdat module
 *
 *  \author Olivia Quinet
 */

#include "reader.hpp"
#include "version.hpp"
#include <Rcpp.h>

RCPP_MODULE(Rcppsas7bdat) {
  ::Rcpp::function("getVersion", &::Rcppsas7bdat::getVersion);

  ::Rcpp::function("Reader", &::Rcppsas7bdat::Reader::build);
  ::Rcpp::class_<::Rcppsas7bdat::Reader>("Reader")
      .constructor<std::string, SEXP, SEXP, SEXP>()
      .method("properties", &::Rcppsas7bdat::Reader::properties)
      .method("read_all", &::Rcppsas7bdat::Reader::read_all)
      .method("read_row", &::Rcppsas7bdat::Reader::read_row)
      .method("read_rows", &::Rcppsas7bdat::Reader::read_rows)
      .method("current_row_index", &::Rcppsas7bdat::Reader::current_row_index)
      .method("skip", &::Rcppsas7bdat::Reader::skip)
      .method("end_of_data", &::Rcppsas7bdat::Reader::end_of_data);
}
