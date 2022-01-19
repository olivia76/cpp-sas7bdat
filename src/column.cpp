/**
 *  \file src/column.cpp
 * 
 *  \brief Column file
 *
 *  \author Olivia Quinet
 */

#include <cppsas7bdat/column.hpp>

namespace {
  cppsas7bdat::COLUMNS get_columns(const cppsas7bdat::COLUMNS& _columns, cppsas7bdat::Column::Type _type)
  {
    cppsas7bdat::COLUMNS columns;
    for(auto column: _columns) {
      if(column.type == _type) columns.emplace_back(column);
    }    
    return columns;
  }
}

namespace cppsas7bdat {
  
  Columns::Columns(const COLUMNS& _columns)
    : strings(get_columns(_columns, Column::Type::string)),
      numbers(get_columns(_columns, Column::Type::number)),
      integers(get_columns(_columns, Column::Type::integer)),
      datetimes(get_columns(_columns, Column::Type::datetime)),
      dates(get_columns(_columns, Column::Type::date)),
      times(get_columns(_columns, Column::Type::time))
  {
  }

}
