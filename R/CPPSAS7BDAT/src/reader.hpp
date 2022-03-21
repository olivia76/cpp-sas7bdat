/**
 *  \file reader.hpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#include <Rcpp.h>
#include <cppsas7bdat/reader.hpp>

namespace Rcppsas7bdat {

class Reader : public cppsas7bdat::Reader {
public:
  static SEXP build(std::string inputfilename, SEXP sink, SEXP include,
                    SEXP exclude);

  Reader(std::string inputfilename, SEXP sink, SEXP include, SEXP exclude);

  SEXP properties() const;

  void read_all();
  bool read_row();
  bool read_rows(const size_t _chunk_size);
  size_t current_row_index() const;
  bool skip(const size_t _nrows);
  void end_of_data();

protected:
  static cppsas7bdat::Reader _build(std::string _inputfilename, SEXP _sink,
                                    SEXP _include, SEXP _exclude);
};

} // namespace Rcppsas7bdat
