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

  Reader::Reader(PSOURCE&& _source, PSINK&& _sink)
    : m_pimpl(impl::build(std::move(_source), std::move(_sink)))
  {
  }

  Reader::Reader(Reader&&) noexcept = default;
  Reader& Reader::operator=(Reader&&) noexcept = default;
  
  Reader::~Reader() = default;
  
  Reader::DataSourceConcept::~DataSourceConcept() = default;
  Reader::DatasetSinkConcept::~DatasetSinkConcept() = default;

  const Properties& Reader::properties() const noexcept
  {
    return m_pimpl->properties();
  }
  
  void Reader::read_all()
  {
    m_pimpl->read_all();
  }
  
  bool Reader::read_row()
  {
    return m_pimpl->read_row();
  }
  
  bool Reader::read_rows(const size_t _chunk_size)
  {
    return m_pimpl->read_rows(_chunk_size);
  }
  
  size_t Reader::current_row_index() const noexcept
  {
    return m_pimpl->current_row_index();
  }
  
  Column::PBUF Reader::read_row_no_sink()
  {
    return m_pimpl->read_row_no_sink();
  }

  Reader::PIMPL Reader::impl::build(PSOURCE&& _source, PSINK&& _sink)
  {
    Properties properties;
    auto rd = READ::data(std::move(_source), &properties.header, &properties.metadata);
    return std::visit([&](auto&& arg) -> Reader::PIMPL {
			using T = std::decay_t<decltype(arg)>;
			using RI = INTERNAL::ReaderImpl<T>;
			return std::make_unique<RI>(std::move(arg), std::move(_sink), std::move(properties));
      }, std::move(rd));
  }
}
