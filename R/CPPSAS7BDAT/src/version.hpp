/**
 *  \file version.hpp
 *
 *  \brief  Version export
 *
 *  \author Olivia Quinet
 */

#include <Rcpp.h>

namespace Rcppsas7bdat {
extern "C" {
SEXP getVersion();
}
} // namespace Rcppsas7bdat
