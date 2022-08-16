/**
 *  \file filter.hpp
 *
 *  \brief  Reader export
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_R_FILTER_HPP_
#define _CPP_SAS7BDAT_R_FILTER_HPP_

#include <Rcpp.h>
#include <cppsas7bdat/filter/column.hpp>

namespace Rcppsas7bdat {

cppsas7bdat::ColumnFilter::IncludeExclude filter(SEXP _include, SEXP _exclude);

} // namespace Rcppsas7bdat

#endif
