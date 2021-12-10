/**
 *  \file src/sas7bdat.cpp
 * 
 *  \brief Main file
 *
 *  \author Olivia Quinet
 */

#include <cppsas7bdat/sas7bdat.hpp>
#include "sas7bdat-impl.hpp"

namespace cppsas7bdat {

  Reader::~Reader() = default;

  Reader::DatasetReaderConcept::DatasetReaderConcept(const char* _pcszFileName)
    : pinternal(std::make_unique<Reader::DatasetReaderConcept::Internal>(_pcszFileName))
  {
  }
  
  /*Reader::DatasetReaderConcept::DatasetReaderConcept(const char* _pcszFileName)
    : pinternal(new Internal(_pcszFileName)),
      properties(pinternal->read_properties(_pcszFileName))
  {
  }*/

  Reader::DatasetReaderConcept::~DatasetReaderConcept() = default;

  const Properties& Reader::DatasetReaderConcept::properties() const noexcept
  {
    return pinternal->properties();
  }

  const COLUMNS& Reader::DatasetReaderConcept::columns() const noexcept
  {
    return properties().metadata.columns;
  }

  size_t Reader::DatasetReaderConcept::current_row_index() const noexcept
  {
    return pinternal->current_row_index();
  }

  bool Reader::DatasetReaderConcept::read()
  {
    auto vals = pinternal->read();
    if(!vals) return false;
    read_row(vals->data());
    return true;
  }
  
}
