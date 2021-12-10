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
#include <fmt/core.h>
#include <variant>
//#include <type_traits>
//#include <map>
//#include <byteswap.h>
//#include <endian.h>
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

namespace cppsas7bdat {
  namespace INTERNAL {

    CHECK_HEADER check_header(const char* _pcszFileName, Properties::Header* _header)
    {
      INTERNAL::CHECK_HEADER ch(_pcszFileName);
      ch.check_magic_number();
      ch.set_aligns_endianness(_header);
      return ch;
    }

    template<Endian _endian, Format _format>
    READ_HEADER<_endian, _format> _read_header(CHECK_HEADER&& ch, Properties::Header* _header)
    {
      READ_HEADER<_endian, _format> rh(std::move(ch));
      rh.set_header_length_and_read(_header);
      rh.set_header(_header);
      return rh;
    }

    template<Endian _endian, Format _format>
    READ_METADATA<_endian, _format> _read_metadata(READ_HEADER<_endian, _format>&& rh,
						   const Properties::Header* _header,
						   Properties::Metadata* _metadata)
    {
      READ_METADATA<_endian, _format> rm(std::move(rh), _header);
      rm.set_metadata(_metadata);
      return rm;
    }

    template<Endian _endian, Format _format, typename _Decompressor>
    READ_DATA<_endian, _format, _Decompressor> _read_data(READ_METADATA<_endian, _format>&& rm,
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
    
    RH read_header(CHECK_HEADER&& ch,
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

    RH read_header(const char* _pcszFileName,
		   Properties::Header* _header)
    {
      return read_header(check_header(_pcszFileName, _header), _header);
    }

    RM read_metadata(RH&& rh,
		     const Properties::Header* _header,
		     Properties::Metadata* _metadata)
    {
      return std::visit([&](auto&& arg) -> RM {
			  using T = std::decay_t<decltype(arg)>;
			  return _read_metadata<T::endian, T::format>(std::move(arg), _header, _metadata);
			}, rh);
    }

    RM read_metadata(const char* _pcszFileName,
		     Properties::Header* _header,
		     Properties::Metadata* _metadata)
    {
      return read_metadata(read_header(_pcszFileName, _header), _header, _metadata);
    }

    RD read_data(RM&& rm,
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

    RD read_data(const char* _pcszFileName,
		 Properties::Header* _header,
		 Properties::Metadata* _metadata)
    {
      return read_data(read_metadata(read_header(_pcszFileName, _header), _header, _metadata), _metadata);
    }

    auto read_line(RD& rd)
    {
      return std::visit([&](auto&& arg) {
			  return arg.read_line();
			}, rd);
    }
    
  }

  class Reader::DatasetReaderConcept::Internal {
  public:
    explicit Internal(const char* _pcszFileName)
      : m_read_data(INTERNAL::read_data(_pcszFileName, &m_properties.header, &m_properties.metadata))
    {
    }

    const Properties& properties() const noexcept { return m_properties; }
    auto read() { ++m_current_row_index; return INTERNAL::read_line(m_read_data); }
    size_t current_row_index() const noexcept { return m_current_row_index; }
    
  private:
    Properties m_properties;
    INTERNAL::RD m_read_data;
    size_t m_current_row_index{std::numeric_limits<std::size_t>::max()};
  };

}

#endif

