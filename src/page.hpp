/**
 *  \file src/page.hpp
 * 
 *  \brief Page reading
 *
 *  \author Olivia Quinet
 */


#ifndef _CPP_SAS7BDAT_SRC_PAGE_HPP_
#define _CPP_SAS7BDAT_SRC_PAGE_HPP_

namespace cppsas7bdat {
  namespace INTERNAL {

    constexpr const uint16_t PAGE_INVALID_TYPE{std::numeric_limits<uint16_t>::max()};
    constexpr const uint16_t PAGE_META_TYPE{0};
    constexpr const uint16_t PAGE_DATA_TYPE{256};
    constexpr const uint16_t PAGE_MIX_TYPE1{512};
    constexpr const uint16_t PAGE_MIX_TYPE2{640};
    constexpr const uint16_t PAGE_AMD_TYPE{1024};
    constexpr const uint16_t PAGE_METC_TYPE{16384};
    
    inline bool is_page_mix(const uint16_t _page) noexcept
    {
      return _page == PAGE_MIX_TYPE1 || _page == PAGE_MIX_TYPE2;
    }

    inline bool is_page_meta_mix_amd(const uint16_t _page) noexcept
    {
      return _page == PAGE_META_TYPE || _page == PAGE_AMD_TYPE || is_page_mix(_page);
    }

    inline bool is_page_data_mix(const uint16_t _page) noexcept
    {
      return _page == PAGE_DATA_TYPE || is_page_mix(_page);
    }
    
    inline bool is_page_meta_data_mix(const uint16_t _page) noexcept
    {
      return _page == PAGE_META_TYPE || _page == PAGE_DATA_TYPE || is_page_mix(_page);
    }

    
    template<Format _format>
    struct PAGE_CONSTANT;

    template<>
    struct PAGE_CONSTANT<Format::bit64> {
      static constexpr const size_t page_bit_offset = 32;
    };

    template<>
    struct PAGE_CONSTANT<Format::bit32> {
      static constexpr const size_t page_bit_offset = 16;
    };
    
    template<Endian _endian, Format _format>
    struct READ_PAGE : public PAGE_CONSTANT<_format> {
      constexpr static auto endian=_endian;
      constexpr static auto format=_format;

      using PAGE_CONSTANT<_format>::page_bit_offset;

      std::ifstream is;
      INTERNAL::BUFFER<_endian, _format> buf;
      const Properties::Header* header;
      struct PAGE_HEADER {
	uint16_t  type{PAGE_INVALID_TYPE};
	uint16_t block_count{0};
	uint16_t subheaders_count{0};

	void reset() {
	  type = PAGE_INVALID_TYPE;
	  block_count = 0;
	  subheaders_count = 0;
	}
      } current_page_header;

      READ_PAGE(std::ifstream&& _is,
		INTERNAL::BUFFER<_endian, _format>&& _buf,
		const Properties::Header* _header)
	: is(std::move(_is)),
	  buf(std::move(_buf)),
	  header(_header)
      {
      }

      READ_PAGE(READ_PAGE&& _rp)
	: is(std::move(_rp.is)),
	  buf(std::move(_rp.buf)),
	  header(_rp.header),
	  current_page_header(_rp.current_page_header)
      {
      }
      
      bool read_page() {
	current_page_header.reset();
	if(!is) return false;
	if(is.eof()) return false;
	D(spdlog::debug("read_page: length={}\n", header->page_length));
	if(!buf.read_stream(is, header->page_length)) {
	  if(is.eof()) return false;
	  EXCEPTION::cannot_read_page();
	}
	return _get_page_header();
      }
      
      bool _get_page_header() {
	D(spdlog::debug("get_page_header: "));
	current_page_header.type             = buf.get_uint16(page_bit_offset+0);
	current_page_header.block_count      = buf.get_uint16(page_bit_offset+2);
	current_page_header.subheaders_count = buf.get_uint16(page_bit_offset+4);
	D(spdlog::debug("{},{},{}\n", current_page_header.type, current_page_header.block_count, current_page_header.subheaders_count));
	return true;
      }

      template<typename _Fct>
      void process_page_subheaders(_Fct _process) {
	constexpr const size_t subheader_size = 3*buf.integer_size; // 12 or 24
	for(uint16_t isubheader = 0; isubheader<current_page_header.subheaders_count; ++isubheader) {
	  const size_t offset = page_bit_offset + 8 + subheader_size*isubheader;
	  D(spdlog::debug("process_page_subheaders: subheader#{}: offset={}, ", isubheader, offset));

	  const auto subheader = _get_page_subheader(offset);
	  
	  if(!subheader.length) continue;
	  if(subheader.compression == TRUNCATED_SUBHEADER_ID) continue;

	  _process(subheader);
	}
      }

      PAGE_SUBHEADER _get_page_subheader(const size_t _offset) {
	const auto offset      = buf.get_uinteger(_offset);
	const auto length      = buf.get_uinteger(_offset + buf.integer_size);
	const auto compression = buf.get_byte    (_offset + buf.integer_size*2);
	const auto type        = buf.get_byte    (_offset + buf.integer_size*2+1);
	D(spdlog::debug("soffset={}, slength={}, scompression={}, stype:{}\n", offset, length, compression, type));
	return PAGE_SUBHEADER{offset, length, compression, type};
      }
      
    };
  }
}

#endif
