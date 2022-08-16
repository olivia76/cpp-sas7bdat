/**
 *  \file reader.cpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

// clang-format off
#include "reader.hpp"
#include "sink_base.hpp"
#include "sink.hpp"
#include "sink_chunk.hpp"
#include "sink_data.hpp"
#include "filter.hpp"
#include <cppsas7bdat/types.hpp>
#include <cppsas7bdat/source/ifstream.hpp>
// clang-format on

using namespace Rcpp;

Rcppsas7bdat::Reader::Reader(std::string _inputfilename, SEXP _sink,
                             SEXP _include, SEXP _exclude)
    : cppsas7bdat::Reader(_build(_inputfilename, _sink, _include, _exclude)) {}

cppsas7bdat::Reader Rcppsas7bdat::Reader::_build(std::string _inputfilename,
                                                 SEXP _sink, SEXP _include,
                                                 SEXP _exclude) {
  Environment sink(_sink);

  if (sink.exists("set_properties") && sink.exists("chunk_size") &&
      sink.exists("push_rows")) {
    const size_t chunk_size = sink["chunk_size"];
    return cppsas7bdat::Reader(
        cppsas7bdat::datasource::ifstream(_inputfilename.c_str()),
        SinkChunk(sink, chunk_size), filter(_include, _exclude));
  } else if (sink.exists("set_properties") && sink.exists("set_data")) {
    return cppsas7bdat::Reader(
        cppsas7bdat::datasource::ifstream(_inputfilename.c_str()),
        SinkData(sink), filter(_include, _exclude));
  } else if (sink.exists("set_properties") && sink.exists("push_row")) {
    return cppsas7bdat::Reader(
        cppsas7bdat::datasource::ifstream(_inputfilename.c_str()), Sink(sink),
        filter(_include, _exclude));
  } else {
    throw std::runtime_error("Not a valid sink");
  }
}

SEXP Rcppsas7bdat::Reader::build(std::string _inputfilename, SEXP _sink,
                                 SEXP _include, SEXP _exclude) {
  return Rcpp::wrap_object(Reader(_inputfilename, _sink, _include, _exclude));
}

SEXP Rcppsas7bdat::Reader::properties() const {
  return Rcpp::wrap_properties(cppsas7bdat::Reader::properties());
}

void Rcppsas7bdat::Reader::read_all() {
  return cppsas7bdat::Reader::read_all();
}

bool Rcppsas7bdat::Reader::read_row() {
  return cppsas7bdat::Reader::read_row();
}

bool Rcppsas7bdat::Reader::read_rows(const size_t _chunk_size) {
  return cppsas7bdat::Reader::read_rows(_chunk_size);
}

size_t Rcppsas7bdat::Reader::current_row_index() const {
  return cppsas7bdat::Reader::current_row_index();
}

bool Rcppsas7bdat::Reader::skip(const size_t _nrows) {
  return cppsas7bdat::Reader::skip(_nrows);
}

void Rcppsas7bdat::Reader::end_of_data() { cppsas7bdat::Reader::end_of_data(); }
