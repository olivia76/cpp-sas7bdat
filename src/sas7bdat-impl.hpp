/**
 *  \file src/sas7bdat-impl.hpp
 * 
 *  \brief Main file
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_SRC_SAS7BDAT_IMPL_HPP_
#define _CPP_SAS7BDAT_SRC_SAS7BDAT_IMPL_HPP_

#include <cppsas7bdat/sas7bdat.hpp>
#include <spdlog/spdlog.h>
//#include <fmt/core.h>
//#include <fmt/ostream.h>
#include <variant>
#include "debug.hpp"
#include "exceptions.hpp"
#include "memory.hpp"
#include "types.hpp"
#include "stream.hpp"
#include "buffer.hpp"
#include "header.hpp"
#include "subheaders.hpp"
#include "page.hpp"
#include "formatters.hpp"
#include "metadata.hpp"
#include "decompressors.hpp"
#include "data.hpp"

/**
 *  Reading a SAS7BDAT is done in several consecutive steps:
 *  - checking the header
 *  - reading the header
 *  - reading the metadata page
 *  - reading the data pages
 *
 *  Each step depends on the completion of the previous step.  In the
 *  following code, the result of the previous step is moved to
 *  construct the next step.
 */
namespace cppsas7bdat {
  namespace INTERNAL {

    inline CHECK_HEADER check_header(const char* _pcszFileName, Properties::Header* _header)
    {
      INTERNAL::CHECK_HEADER ch(_pcszFileName);
      ch.check_magic_number();
      ch.set_aligns_endianness(_header);
      return ch;
    }

    template<Endian _endian, Format _format>
    inline READ_HEADER<_endian, _format> _read_header(CHECK_HEADER&& ch, Properties::Header* _header)
    {
      READ_HEADER<_endian, _format> rh(std::move(ch));
      rh.set_header_length_and_read(_header);
      rh.set_header(_header);
      return rh;
    }

    template<Endian _endian, Format _format>
    inline READ_METADATA<_endian, _format> _read_metadata(READ_HEADER<_endian, _format>&& rh,
							  const Properties::Header* _header,
							  Properties::Metadata* _metadata)
    {
      READ_METADATA<_endian, _format> rm(std::move(rh), _header);
      rm.set_metadata(_metadata);
      return rm;
    }

    template<Endian _endian, Format _format, typename _Decompressor>
    inline READ_DATA<_endian, _format, _Decompressor> _read_data(READ_METADATA<_endian, _format>&& rm,
								 _Decompressor&& _decompressor,
								 const Properties::Metadata* _metadata)
    {
      READ_DATA<_endian, _format, _Decompressor> rd(std::move(rm), std::move(_decompressor), _metadata);
      return rd;
    }

    using RH = std::variant< READ_HEADER<Endian::big, Format::bit64>,
			     READ_HEADER<Endian::big, Format::bit32>,
			     READ_HEADER<Endian::little, Format::bit64>,
			     READ_HEADER<Endian::little, Format::bit32> >;
    using RM = std::variant< READ_METADATA<Endian::big, Format::bit64>,
			     READ_METADATA<Endian::big, Format::bit32>,
			     READ_METADATA<Endian::little, Format::bit64>,
			     READ_METADATA<Endian::little, Format::bit32> >;
    using RD = std::variant< READ_DATA<Endian::big, Format::bit64, DECOMPRESSOR::None>,
			     READ_DATA<Endian::big, Format::bit64, DECOMPRESSOR::RDC<Endian::big, Format::bit64> >,
			     READ_DATA<Endian::big, Format::bit64, DECOMPRESSOR::RLE<Endian::big, Format::bit64> >,
			     READ_DATA<Endian::big, Format::bit32, DECOMPRESSOR::None>,
			     READ_DATA<Endian::big, Format::bit32, DECOMPRESSOR::RDC<Endian::big, Format::bit32> >,
			     READ_DATA<Endian::big, Format::bit32, DECOMPRESSOR::RLE<Endian::big, Format::bit32> >,
			     READ_DATA<Endian::little, Format::bit64, DECOMPRESSOR::None>,
			     READ_DATA<Endian::little, Format::bit64, DECOMPRESSOR::RDC<Endian::little, Format::bit64> >,
			     READ_DATA<Endian::little, Format::bit64, DECOMPRESSOR::RLE<Endian::little, Format::bit64>>,
			     READ_DATA<Endian::little, Format::bit32, DECOMPRESSOR::None>,
			     READ_DATA<Endian::little, Format::bit32, DECOMPRESSOR::RDC<Endian::little, Format::bit32> >,
			     READ_DATA<Endian::little, Format::bit32, DECOMPRESSOR::RLE<Endian::little, Format::bit32>> >;
    
    inline RH read_header(CHECK_HEADER&& ch,
			  Properties::Header* _header)
    {
      if(ch.is_big_endian) {
	if(ch.is_64bit) return _read_header<Endian::big, Format::bit64>(std::move(ch), _header);
	else return _read_header<Endian::big, Format::bit32>(std::move(ch), _header);
      } else {
	if(ch.is_64bit) return _read_header<Endian::little, Format::bit64>(std::move(ch), _header);
	else return _read_header<Endian::little, Format::bit32>(std::move(ch), _header);
      }
    }

    inline RH read_header(const char* _pcszFileName,
			  Properties::Header* _header)
    {
      return read_header(check_header(_pcszFileName, _header), _header);
    }

    inline RM read_metadata(RH&& rh,
			    const Properties::Header* _header,
			    Properties::Metadata* _metadata)
    {
      return std::visit([&](auto&& arg) -> RM {
			  using T = std::decay_t<decltype(arg)>;
			  return _read_metadata<T::endian, T::format>(std::move(arg), _header, _metadata);
			}, rh);
    }

    inline RM read_metadata(const char* _pcszFileName,
			    Properties::Header* _header,
			    Properties::Metadata* _metadata)
    {
      return read_metadata(read_header(_pcszFileName, _header), _header, _metadata);
    }

    inline RD read_data(RM&& rm,
			const Properties::Metadata* _metadata)
    {
      return std::visit([&](auto&& arg) -> RD {
			  using T = std::decay_t<decltype(arg)>;
			  switch(_metadata->compression) {
			  case Compression::RDC: return _read_data<T::endian, T::format>(std::move(arg), DECOMPRESSOR::RDC<T::endian, T::format>(_metadata), _metadata);
			  case Compression::RLE: return _read_data<T::endian, T::format>(std::move(arg), DECOMPRESSOR::RLE<T::endian, T::format>(_metadata), _metadata);
			  default:
			    return _read_data<T::endian, T::format>(std::move(arg), DECOMPRESSOR::None(), _metadata);
			  }
			}, rm);
    }

    inline RD read_data(const char* _pcszFileName,
			Properties::Header* _header,
			Properties::Metadata* _metadata)
    {
      return read_data(read_metadata(read_header(_pcszFileName, _header), _header, _metadata), _metadata);
    }

    inline auto read_line(RD& rd)
    {
      return std::visit([&](auto&& arg) {
			  return arg.read_line();
			}, rd);
    }
    
  }

  class Reader::DatasetSinkConcept::Internal {
  public:
    explicit Internal(const char* _pcszFileName)
      : m_read_data(INTERNAL::read_data(_pcszFileName, &m_properties.header, &m_properties.metadata))
    {
    }

    const Properties& properties() const noexcept { return m_properties; }
    auto read_row() { ++m_current_row_index; return INTERNAL::read_line(m_read_data); }
    size_t current_row_index() const noexcept { return m_current_row_index; }
    
  private:
    Properties m_properties;
    INTERNAL::RD m_read_data;
    size_t m_current_row_index{std::numeric_limits<std::size_t>::max()};
    static_assert((std::numeric_limits<std::size_t>::max()+1) == 0);
  };

}

#endif

