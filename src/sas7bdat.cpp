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

  std::string_view to_string(const Endian _x)
  {
    switch(_x) {
    case Endian::little: return "little";
    case Endian::big: return "big";
    default:
      return "unknown";
    }
  }
  
  std::string_view to_string(const Format _x)
  {
    switch(_x) {
    case Format::bit32: return "32bits";
    case Format::bit64: return "64bits";
    default:
      return "unknown";
    }
  }
  
  std::string_view to_string(const Platform _x)
  {
    switch(_x) {
    case Platform::unix: return "unix";
    case Platform::windows: return "windows";
    default:
      return "unknown";
    }
  }
  
  std::string_view to_string(const Compression _x)
  {
    switch(_x) {
    case Compression::none: return "none";
    case Compression::RLE: return "RLE";
    case Compression::RDC: return "RDC";
    default:
      return "unknown";
    }
  }
  
  std::string_view to_string(const Column::Type _x)
  {
    switch(_x) {
    case Column::Type::string: return "string";
    case Column::Type::number: return "number";
    case Column::Type::integer: return "integer";
    case Column::Type::datetime: return "datetime";
    case Column::Type::date: return "date";
    case Column::Type::time: return "time";
    default:
      return "unknown";
    }    
  }
  
  std::ostream& operator<<(std::ostream& os, const Endian _x)
  {
    os << to_string(_x);
    return os;
  }
  
  std::ostream& operator<<(std::ostream& os, const Format _x)
  {
    os << to_string(_x);
    return os;
  }
  
  std::ostream& operator<<(std::ostream& os, const Platform _x)
  {
    os << to_string(_x);
    return os;
  }
  
  std::ostream& operator<<(std::ostream& os, const Compression _x)
  {
    os << to_string(_x);
    return os;
  }  

  std::ostream& operator<<(std::ostream& os, const Column::Type _x)
  {
    os << to_string(_x);
    return os;
  }
  
  Reader::~Reader() = default;

  Reader::DatasetSinkConcept::DatasetSinkConcept(const char* _pcszFileName)
    : pinternal(std::make_unique<Reader::DatasetSinkConcept::Internal>(_pcszFileName))
  {
  }
  
  /*Reader::DatasetSinkConcept::DatasetSinkConcept(const char* _pcszFileName)
    : pinternal(new Internal(_pcszFileName)),
      properties(pinternal->read_properties(_pcszFileName))
  {
  }*/

  Reader::DatasetSinkConcept::~DatasetSinkConcept() = default;

  const Properties& Reader::DatasetSinkConcept::properties() const noexcept
  {
    return pinternal->properties();
  }

  const COLUMNS& Reader::DatasetSinkConcept::columns() const noexcept
  {
    return properties().metadata.columns;
  }

  size_t Reader::DatasetSinkConcept::current_row_index() const noexcept
  {
    return pinternal->current_row_index();
  }

  void Reader::DatasetSinkConcept::read_all()
  {
    while(read_row());
  }
  
  bool Reader::DatasetSinkConcept::read_row()
  {
    auto vals = pinternal->read_row();
    if(!vals) return false;
    read_row(vals->data());
    return true;
  }
  
}
