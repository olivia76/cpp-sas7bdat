/**
 *  \file src/data.hpp
 * 
 *  \brief Data reading
 *
 *  \author Olivia Quinet
 */


#ifndef _CPP_SAS7BDAT_SRC_DATA_HPP_
#define _CPP_SAS7BDAT_SRC_DATA_HPP_

#include <optional>		

namespace cppsas7bdat {
  namespace INTERNAL {

    template<Endian _endian, Format _format, typename _Decompressor>
    struct READ_DATA : public READ_PAGE<_endian, _format> {
      constexpr static auto endian=_endian;
      constexpr static auto format=_format;

      constexpr static size_t integer_size = BUFFER<_endian, _format>::integer_size;

      mutable _Decompressor decompressor;
      DATASUBHEADERS data_subheaders;
      const Properties::Metadata* metadata;
      size_t current_row{0};
      size_t current_row_on_page{0};
      
      using READ_PAGE<_endian, _format>::header;
      using READ_PAGE<_endian, _format>::read_page;
      using READ_PAGE<_endian, _format>::process_page_subheaders;
      using READ_PAGE<_endian, _format>::current_page_header;
      using READ_PAGE<_endian, _format>::buf;
      
      using PAGE_CONSTANT<_format>::page_bit_offset;
      
      READ_DATA(READ_METADATA<_endian, _format>&& _rm, _Decompressor&& _decompressor, const Properties::Metadata* _metadata)
	: READ_PAGE<_endian, _format>(std::move(_rm)),
	  decompressor(std::move(_decompressor)),
	  data_subheaders(std::move(_rm.data_subheaders)),
	  metadata(_metadata)
      {
      }

      bool next() {
	if(current_row == metadata->row_count) return false;

	// Do we need to read a new page from the file?
	if(
	   (current_page_header.type == PAGE_META_TYPE && current_row_on_page == data_subheaders.size()) ||
	   (current_page_header.type == PAGE_DATA_TYPE && current_row_on_page == current_page_header.block_count) ||
	   (is_page_mix(current_page_header.type) && current_row_on_page == std::min(metadata->row_count, metadata->mix_page_row_count))
	   ) {
	  if(!read_next_page()) return false;
	}
	return true;
      }      

      std::optional<BYTES> read_line() {
	if(!next()) return {};

	size_t offset{0};
	size_t length{metadata->row_length};
	
	if(current_page_header.type == PAGE_META_TYPE) {
	  D(spdlog::debug("READ_DATA::read_line::PAGE_META_TYPE: {}, {}\n", current_row, current_row_on_page));
	  auto data_subheader = data_subheaders[current_row_on_page];
	  offset = data_subheader.offset;
	  length = data_subheader.length;
	} else if(current_page_header.type == PAGE_DATA_TYPE) {
	  D(spdlog::debug("READ_DATA::read_line::PAGE_DATA_TYPE: {}, {}\n", current_row, current_row_on_page));
	  offset = page_bit_offset + 8 + metadata->row_length * current_row_on_page;
	} else if(is_page_mix(current_page_header.type)) {
	  D(spdlog::debug("READ_DATA::read_line::PAGE_MIX_TYPE: {}, {}\n", current_row, current_row_on_page));
	  constexpr const size_t subheader_size = 3*buf.integer_size; // 12 or 24
	  const size_t align_correction = (page_bit_offset + 8 +current_page_header.subheaders_count * subheader_size) % 8;
	  offset = page_bit_offset + 8 + align_correction + current_page_header.subheaders_count * subheader_size + metadata->row_length * current_row_on_page;
	} else {
	  spdlog::debug("Unknown page type: {}\n", current_page_header.type);
	  return {};
	}
	++current_row;
	++current_row_on_page;
	
	return extract_row_values(offset, length);
      }
      
      BYTES extract_row_values(const size_t _offset, size_t _length) const
      {
	D(spdlog::debug("READ_DATA::extract_row_values({}, {})\n", _offset, _length));
	assert(_offset + _length <= header->page_length);
	auto values = buf.get_bytes(_offset, _length);
	const auto row_length = metadata->row_length;
	if(_length < row_length && metadata->compression != Compression::none) {
	  return decompressor(values);
	}
	return values;
      }

      bool read_next_page() {
	D(spdlog::debug("read_next_page\n"));
	data_subheaders.clear();
	current_row_on_page = 0;
	while(true) {
	  if(!read_page()) return false;
	  if(current_page_header.type == PAGE_META_TYPE) {
	    process_page_metadata();
	    if(!data_subheaders.empty()) return true;
	  } else if(is_page_data_mix(current_page_header.type)) return true;
	}
      }

      void process_page_metadata() {
	auto psh = [&](const PAGE_SUBHEADER& _subheader) {
		     if(DataSubHeader::check(metadata, _subheader)) process_DATA_SUBHEADER(_subheader);
		   };
	process_page_subheaders(psh);
      }

      void process_DATA_SUBHEADER([[maybe_unused]] const PAGE_SUBHEADER& _subheader)
      {
	D(spdlog::debug("READ_DATA::process_DATA_SUBHEADER: {}, {}\n", _subheader.offset, _subheader.length));
	data_subheaders.emplace_back(_subheader);
      }
      
    };
    
  }
}

#endif
