/**
 *  \file cppsas7bdat/column-filter.hpp
 * 
 *  \brief Column filter description
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_COLUMN_FILTER_HPP_
#define _CPP_SAS7BDAT_COLUMN_FILTER_HPP_

#include <set>
#include <string>

#include <cppsas7bdat/column.hpp>

namespace cppsas7bdat {

  namespace ColumnFilter {
  
    struct AcceptAll {
      bool accept([[maybe_unused]] const Column& _column) const noexcept { return true; }
    };
  
    struct IncludeExclude {
      using NAMES = std::set<std::string>;
      
      NAMES included;
      NAMES excluded;
      
      bool accept(const Column& _column) const noexcept
      {
	return is_accepted(_column.name);
      }

      bool is_accepted(const std::string& _name) const noexcept
      {
	if(!included.empty()) return is_included(_name);
	if(!excluded.empty()) return !is_excluded(_name);
	return true;
      }
      
      bool is_included(const std::string& _name) const noexcept
      {
	return included.find(_name) != included.end();
      }

      bool is_excluded(const std::string& _name) const noexcept
      {
	return excluded.find(_name) != excluded.end();
      }
    };
    
  }
  
}


#endif
