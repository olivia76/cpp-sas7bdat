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

    using DATASOURCE = Reader::PSOURCE;
    
    inline CHECK_HEADER<DATASOURCE> check_header(DATASOURCE&& _source, Properties::Header* _header)
    {
      INTERNAL::CHECK_HEADER<DATASOURCE> ch(std::move(_source));
      ch.check_magic_number();
      ch.set_aligns_endianness(_header);
      return ch;
    }

    template<Endian _endian, Format _format>
    inline READ_HEADER<DATASOURCE, _endian, _format> _read_header(CHECK_HEADER<DATASOURCE>&& ch, Properties::Header* _header)
    {
      READ_HEADER<DATASOURCE, _endian, _format> rh(std::move(ch));
      rh.set_header_length_and_read(_header);
      rh.set_header(_header);
      return rh;
    }

    template<Endian _endian, Format _format>
    inline READ_METADATA<DATASOURCE, _endian, _format> _read_metadata(READ_HEADER<DATASOURCE, _endian, _format>&& rh,
								      const Properties::Header* _header,
								      Properties::Metadata* _metadata)
    {
      READ_METADATA<DATASOURCE, _endian, _format> rm(std::move(rh), _header);
      rm.set_metadata(_metadata);
      return rm;
    }

    template<Endian _endian, Format _format, typename _Decompressor>
    inline READ_DATA<DATASOURCE, _endian, _format, _Decompressor> _read_data(READ_METADATA<DATASOURCE, _endian, _format>&& rm,
									     _Decompressor&& _decompressor,
									     const Properties::Metadata* _metadata)
    {
      READ_DATA<DATASOURCE, _endian, _format, _Decompressor> rd(std::move(rm), std::move(_decompressor), _metadata);
      return rd;
    }

    using RH = std::variant< READ_HEADER<DATASOURCE, Endian::big, Format::bit64>,
			     READ_HEADER<DATASOURCE, Endian::big, Format::bit32>,
			     READ_HEADER<DATASOURCE, Endian::little, Format::bit64>,
			     READ_HEADER<DATASOURCE, Endian::little, Format::bit32> >;
    using RM = std::variant< READ_METADATA<DATASOURCE, Endian::big, Format::bit64>,
			     READ_METADATA<DATASOURCE, Endian::big, Format::bit32>,
			     READ_METADATA<DATASOURCE, Endian::little, Format::bit64>,
			     READ_METADATA<DATASOURCE, Endian::little, Format::bit32> >;
    using RD = std::variant< READ_DATA<DATASOURCE, Endian::big, Format::bit64, DECOMPRESSOR::None>,
			     READ_DATA<DATASOURCE, Endian::big, Format::bit64, DECOMPRESSOR::RDC<Endian::big, Format::bit64> >,
			     READ_DATA<DATASOURCE, Endian::big, Format::bit64, DECOMPRESSOR::RLE<Endian::big, Format::bit64> >,
			     READ_DATA<DATASOURCE, Endian::big, Format::bit32, DECOMPRESSOR::None>,
			     READ_DATA<DATASOURCE, Endian::big, Format::bit32, DECOMPRESSOR::RDC<Endian::big, Format::bit32> >,
			     READ_DATA<DATASOURCE, Endian::big, Format::bit32, DECOMPRESSOR::RLE<Endian::big, Format::bit32> >,
			     READ_DATA<DATASOURCE, Endian::little, Format::bit64, DECOMPRESSOR::None>,
			     READ_DATA<DATASOURCE, Endian::little, Format::bit64, DECOMPRESSOR::RDC<Endian::little, Format::bit64> >,
			     READ_DATA<DATASOURCE, Endian::little, Format::bit64, DECOMPRESSOR::RLE<Endian::little, Format::bit64>>,
			     READ_DATA<DATASOURCE, Endian::little, Format::bit32, DECOMPRESSOR::None>,
			     READ_DATA<DATASOURCE, Endian::little, Format::bit32, DECOMPRESSOR::RDC<Endian::little, Format::bit32> >,
			     READ_DATA<DATASOURCE, Endian::little, Format::bit32, DECOMPRESSOR::RLE<Endian::little, Format::bit32>> >;
    
    inline RH read_header(CHECK_HEADER<DATASOURCE>&& ch,
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

    inline RM read_metadata(RH&& rh,
			    const Properties::Header* _header,
			    Properties::Metadata* _metadata)
    {
      return std::visit([&](auto&& arg) -> RM {
			  using T = std::decay_t<decltype(arg)>;
			  return _read_metadata<T::endian, T::format>(std::move(arg), _header, _metadata);
			}, rh);
    }

    inline RD read_data(RM&& rm,
			const Properties::Metadata* _metadata)
    {
      return std::visit([&](auto&& arg) -> RD {
			  using T = std::decay_t<decltype(arg)>;
			  switch(_metadata->compression) {
			  case Compression::RDC: return _read_data< T::endian, T::format>(std::move(arg), DECOMPRESSOR::RDC<T::endian, T::format>(_metadata), _metadata);
			  case Compression::RLE: return _read_data< T::endian, T::format>(std::move(arg), DECOMPRESSOR::RLE<T::endian, T::format>(_metadata), _metadata);
			  default:
			    return _read_data<T::endian, T::format>(std::move(arg), DECOMPRESSOR::None(), _metadata);
			  }
			}, rm);
    }

    inline auto read_line(RD& rd)
    {
      return std::visit([&](auto&& arg) {
			  return arg.read_line();
			}, rd);
    }
    
  }

  namespace READ {

    inline INTERNAL::RH header(INTERNAL::DATASOURCE&& _source,
			       Properties::Header* _header)
    {
      return INTERNAL::read_header(INTERNAL::check_header(std::move(_source), _header), _header);
    }

    inline INTERNAL::RM metadata(INTERNAL::DATASOURCE&& _source,
				 Properties::Header* _header,
				 Properties::Metadata* _metadata)
    {
      return INTERNAL::read_metadata(READ::header(std::move(_source), _header), _header, _metadata);
    }

    inline INTERNAL::RD data(INTERNAL::DATASOURCE&& _source,
			     Properties::Header* _header,
			     Properties::Metadata* _metadata)
    {
      return INTERNAL::read_data(INTERNAL::read_metadata(INTERNAL::read_header(INTERNAL::check_header(std::move(_source), _header), _header), _header, _metadata), _metadata);
      //return INTERNAL::read_data(READ::metadata(std::move(_source), _header), _header), _metadata);
    }
    
  }

  class Reader::impl {
  public:

    static PIMPL build(PSOURCE&& _source, PSINK&& _sink)
    {
      return std::make_unique<impl>(std::move(_source), std::move(_sink));
    }
    
    explicit impl(PSOURCE&& _source, PSINK&& _sink)
      :	m_sink(std::move(_sink)),
	m_read_data(READ::data(std::move(_source), &m_properties.header, &m_properties.metadata))
    {
      m_sink->set_properties(properties());
    }
   
    const Properties& properties() const noexcept { return m_properties; }
    size_t current_row_index() const noexcept { return m_current_row_index; }

    void push_row(const size_t _row_index, Column::PBUF _p)
    {
      m_sink->push_row(_row_index, _p);
    }

    bool read_row()
    {
      ++m_current_row_index;
      auto vals = INTERNAL::read_line(m_read_data);
      if(!vals) return false;
      m_sink->push_row(current_row_index(), vals->data());
      return true;
    }

    void read_all()
    {
      while(read_row());
    }
    
  private:
    PSINK m_sink;
    
    Properties m_properties;
    INTERNAL::RD m_read_data;
    size_t m_current_row_index{std::numeric_limits<std::size_t>::max()};
    static_assert((std::numeric_limits<std::size_t>::max()+1) == 0);
  };

}

#endif

