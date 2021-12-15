/**
 *  \file src/header.hpp
 * 
 *  \brief Header reading
 *
 *  \author Olivia Quinet
 */


#ifndef _CPP_SAS7BDAT_SRC_HEADER_HPP_
#define _CPP_SAS7BDAT_SRC_HEADER_HPP_

namespace cppsas7bdat {
  namespace INTERNAL {

    struct CHECK_HEADER {
      static constexpr size_t HEADER_SIZE{288};
      static constexpr BYTE MAGIC_NUMBER[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
					       0x00,0x00,0x00,0x00,0xc2,0xea,0x81,0x60,
					       0xb3,0x14,0x11,0xcf,0xbd,0x92,0x08,0x00,
					       0x09,0xc7,0x31,0x8c,0x18,0x1f,0x10,0x11 };
      
      std::ifstream is;
      INTERNAL::MBUFFER buf;
      size_t align1{0};
      size_t align2{0};
      size_t total_align{0};
      bool is_big_endian{false};
      bool is_64bit{false};

      explicit CHECK_HEADER(const char* _pcszFileName)
	: is(INTERNAL::open_stream(_pcszFileName))
      {
	D(spdlog::debug("Reading header ...\n"));
	if(!buf.read_stream(is, HEADER_SIZE)) EXCEPTION::header_too_short();
	assert(is);
      }

      void check_magic_number() const {
	D(spdlog::debug("Checking magic number ...\n"));
	if(!_check_magic_number()) EXCEPTION::invalid_magic_number();
      }

      bool _check_magic_number() const noexcept {
	return std::memcmp(buf.data(0, sizeof(MAGIC_NUMBER)), MAGIC_NUMBER, sizeof(MAGIC_NUMBER)) == 0;
      }

      void set_aligns_endianness(Properties::Header* _header) {
	if(buf[32] == '3') { align2 = 4; _header->format = Format::bit64; is_64bit = true; }
	if(buf[35] == '3') { align1 = 4; }
	total_align = align1 + align2;
	_header->endianness = (buf[37] == 0x01 ? Endian::little : Endian::big);
	is_big_endian = _header->endianness == Endian::big;
	D(spdlog::debug("Setting aligns ... {}, {}, {}, {}, {}\n", align1, align2, total_align, _header->format, _header->endianness));
      }      
    };

    template<Endian _endian, Format _format>
    struct READ_HEADER {
      constexpr static auto endian=_endian;
      constexpr static auto format=_format;

      std::ifstream is;
      INTERNAL::BUFFER<_endian, _format> buf;
      const size_t align1;
      const size_t total_align;

      READ_HEADER(CHECK_HEADER&& _ch)
	: is(std::move(_ch.is)),
	  buf(std::move(_ch.buf)),
	  align1(_ch.align1),
	  total_align(_ch.total_align)
      {
	assert(is);
      }

      void set_header_length_and_read(Properties::Header* _header) {
	_header->header_length = buf.get_uint32(196+align1); // 196-200 + align1

	// Read the rest of the header
	if(_header->format == Format::bit64 && _header->header_length != 8192) {
	  spdlog::info("Expected header length of 8192 but got {}\n", _header->header_length);
	}

	if(!buf.read_stream(is, _header->header_length-CHECK_HEADER::HEADER_SIZE, CHECK_HEADER::HEADER_SIZE)) EXCEPTION::header_too_short();
	D(spdlog::debug("Set header length and read ... {}\n", _header->header_length));
      }
      
      void set_header(Properties::Header* _header) const {
	_header->platform = (buf[39] == '1' ? Platform::unix :
			     buf[39] == '2' ? Platform::windows : Platform::unknown);
	_header->encoding = get_encoding(buf[70]); // 70
	_header->dataset_name = buf.get_string(92, 64); // 92-156
	_header->file_type = buf.get_string(156, 8); // 156-164

	_header->date_created = get_datetime_from_epoch(buf.get_double(164+align1)); // 164-172 + align1
	_header->date_modified = get_datetime_from_epoch(buf.get_double(172+align1)); // 172-180 + align1

	_header->page_length = buf.get_uint32(200+align1); // 200-204 + align1
	_header->page_count = buf.get_uint32(204+align1); // 204-208 + align1

	_header->sas_release = buf.get_string(216+total_align, 8); // 216-224 + total_align
	_header->sas_server_type = buf.get_string(224+total_align, 16); // 224-240 + total_align
	_header->os_type = buf.get_string(240+total_align, 16); // 240-256
	_header->os_name = ( buf[272+total_align]
			    ? buf.get_string(272+total_align, 16)    // 272+286 + total_align
			    : buf.get_string(256+total_align, 16) ); // 256-272 + total_align

	D(spdlog::debug("Setting header ... {}, {}, {}, {}, {}, {}, {}, {}, {} / {}, {}\n",
			_header->platform,
			_header->dataset_name, _header->file_type,
			boost::posix_time::to_iso_extended_string(_header->date_created),
			boost::posix_time::to_iso_extended_string(_header->date_modified),
			_header->sas_release, _header->sas_server_type, _header->os_type, _header->os_name,
			_header->page_length, _header->page_count));
      }

      static std::string_view get_encoding(const uint8_t _e) noexcept {
	switch(_e) {
	case 20: return "UTF-8";
	case 28: return "US-ASCII";
	case 29: return "ISO-8859-1";
	case 30: return "ISO-8859-2";
	case 31: return "ISO-8859-3";
	case 34: return "ISO-8859-6";
	case 35: return "ISO-8859-7";
	case 36: return "ISO-8859-8";
	case 39: return "ISO-8859-11";
	case 40: return "ISO-8859-9";
	case 60: return "WINDOWS-1250";
	case 61: return "WINDOWS-1251";
	case 62: return "WINDOWS-1252";
	case 63: return "WINDOWS-1253";
	case 64: return "WINDOWS-1254";
	case 65: return "WINDOWS-1255";
	case 66: return "WINDOWS-1256";
	case 67: return "WINDOWS-1257";
	case 118: return "CP950";
	case 119: return "EUC-TW";
	case 123: return "BIG5-HKSCS";
	case 125: return "GB18030";
	case 126: return "CP936";
	case 134: return "EUC-JP";
	case 138: return "CP932";
	case 140: return "EUC-KR";
	case 141: return "CP949";
	default: return "UTF-8";
	};
      }
    };
    
  }
}

#endif
