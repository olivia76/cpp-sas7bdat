/**
 *  \file cppsas7bdat/sas7bdat.hpp
 * 
 *  \brief Properties description
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_PROPERTIES_HPP_
#define _CPP_SAS7BDAT_PROPERTIES_HPP_

#include <string_view>
#include <string>
#include <iosfwd>
#include <cstddef>
#include <cppsas7bdat/column.hpp>

namespace cppsas7bdat {

  enum class Endian {
    unknown, little, big
  };
  enum class Format {
    bit32, bit64 		     
  };
  enum class Platform {
    unknown, unix, windows		       
  };
  enum class Compression {
    none, RLE, RDC
  };

  std::string_view to_string(const Endian _x);
  std::string_view to_string(const Format _x);
  std::string_view to_string(const Platform _x);
  std::string_view to_string(const Compression _x);
  
  std::ostream& operator<<(std::ostream& os, const Endian _x);
  std::ostream& operator<<(std::ostream& os, const Format _x);
  std::ostream& operator<<(std::ostream& os, const Platform _x);
  std::ostream& operator<<(std::ostream& os, const Compression _x);
  
  struct Header {
    Header() = default;
    Header(const Header&) = default;
    Header& operator=(const Header&) = delete;
    Header(Header&&) noexcept = default;
    Header& operator=(Header&&) noexcept = delete;
    
    Format format{Format::bit32};
    Endian endianness{Endian::unknown};
    Platform platform{Platform::unknown};
    DATETIME date_created;
    DATETIME date_modified;
    std::string dataset_name;
    std::string encoding;
    std::string file_type;
    std::string sas_release;
    std::string sas_server_type;
    std::string os_type;
    std::string os_name;
    size_t header_length{0};
    size_t page_length{0};
    size_t page_count{0};
  };
  struct Metadata {
    Metadata() = default;
    Metadata(const Metadata&) = default;
    Metadata& operator=(const Metadata&) = delete;
    Metadata(Metadata&&) noexcept = default;
    Metadata& operator=(Metadata&&) noexcept = delete;
    
    Compression compression{Compression::none};
    std::string creator;
    std::string creator_proc;
    size_t row_length{0};
    size_t row_count{0};
    size_t column_count{0};
    size_t col_count_p1{0};
    size_t col_count_p2{0};
    size_t mix_page_row_count{0};
    size_t lcs{0};
    size_t lcp{0};
    COLUMNS columns;
  };
  static_assert(std::is_copy_constructible_v<Metadata>, "Metadata is not copyable");
  static_assert(!std::is_copy_assignable_v<Metadata>, "Metadata is copy-assignable");
  static_assert(std::is_nothrow_move_constructible_v<Metadata>, "Metadata is not movable");
  static_assert(!std::is_nothrow_move_assignable_v<Metadata>, "Metadata is move-assignable");

  struct Properties : public Header, public Metadata {
    Properties() = default;
    Properties(const Properties&) = default;
    Properties& operator=(const Properties&) = delete;
    Properties(Properties&&) noexcept = default;
    Properties& operator=(Properties&&) noexcept = delete;
    
    std::string filename;
  };
  static_assert(std::is_copy_constructible_v<Properties>, "Properties is not copyable");
  static_assert(!std::is_copy_assignable_v<Properties>, "Properties is copy-assignable");
  static_assert(std::is_nothrow_move_constructible_v<Properties>, "Properties is not movable");
  static_assert(!std::is_nothrow_move_assignable_v<Properties>, "Properties is move-assignable");
  
}

#endif
