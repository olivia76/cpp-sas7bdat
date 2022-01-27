/**
 *  \file reader.hpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#include <Rcpp.h>
#include <cppsas7bdat/sas7bdat.hpp>

namespace Rcppsas7bdat {

  class Reader : public cppsas7bdat::Reader {
  public:
    static SEXP build(std::string _inputfilename, SEXP _sink);
    
    Reader(std::string _inputfilename, SEXP _sink);

    SEXP properties() const;

    void read_all();
    bool read_row();
    bool read_rows(const size_t _chunk_size);
    size_t current_row_index() const;

  protected:
    static cppsas7bdat::Reader _build(std::string _inputfilename, SEXP _sink);
  };
  
}
