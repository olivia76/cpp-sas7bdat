/**
 *  \file version.hpp
 *
 *  \brief  Version export
 *
 *  \author Olivia Quinet
 */

#include "version.hpp"
#include <cppsas7bdat/version.hpp>

using namespace Rcpp;

SEXP Rcppsas7bdat::getVersion() { return wrap(cppsas7bdat::getVersion()); }
