/**
 *  \file cppsas7bdat/sas7bdat.hpp
 * 
 *  \brief Main file
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_SAS7BDAT_HPP_
#define _CPP_SAS7BDAT_SAS7BDAT_HPP_

#include <cppsas7bdat/version.hpp>
#include <cppsas7bdat/column.hpp>

#include <string>
#include <memory>
#include <fstream>
#include <vector>

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
  
  struct Properties {
    std::string filename;
    struct Header {
      Header() = default;
      Header(const Header&) noexcept = delete;
      Header& operator=(const Header&) noexcept = delete;
      Header(Header&&) noexcept = delete;
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
    } header;
    struct Metadata {
      Metadata() = default;
      Metadata(const Metadata&) noexcept = delete;
      Metadata& operator=(const Metadata&) noexcept = delete;
      Metadata(Metadata&&) noexcept = delete;
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
    } metadata;
  };

  class Reader {
  private:
    struct DataSourceConcept {
      virtual ~DataSourceConcept();

      virtual bool eof() = 0;
      virtual bool read_bytes(void* _p, const size_t _length) = 0;
    };

    template<typename _Sp>
    struct DataSourceModel : DataSourceConcept {
      DataSourceModel(_Sp&& _source)
	: source(std::forward<_Sp>(_source))
      {
      }

      bool eof() final {
	return source.eof();
      }

      bool read_bytes(void* _p, const size_t _length) final {
	return source.read_bytes(_p, _length);
      }

      _Sp source;      
    };
    
    struct DatasetSinkConcept {
      virtual ~DatasetSinkConcept();

      virtual void set_properties(const Properties& _properties) = 0;
      virtual void push_row(const size_t _row_index, Column::PBUF _p) = 0;
    };

    template<typename _Dp>
    struct DatasetSinkModel : DatasetSinkConcept {
      DatasetSinkModel(_Dp&& _dataset)
	: dataset(std::forward<_Dp>(_dataset)) {}

      void set_properties(const Properties& _properties) final
      {
	dataset.set_properties(_properties);
      }
      
      void push_row(const size_t _row_index, Column::PBUF _p) final
      {
	dataset.read_row(_row_index, _p);
      }
      
    private:
      _Dp dataset;
    };

  public:
    class impl;
    using PIMPL = std::unique_ptr<impl>;
    using PSOURCE = std::unique_ptr<DataSourceConcept>;
    using PSINK = std::unique_ptr<DatasetSinkConcept>;

    template<typename _Sp>
    static PSOURCE build_source(_Sp&& _source)
    {
      return std::make_unique< DataSourceModel<_Sp> >(std::forward<_Sp>(_source));
    }
    template<typename _Dp>
    static PSINK build_sink(_Dp&& _sink)
    {
      return std::make_unique< DatasetSinkModel<_Dp> >(std::forward<_Dp>(_sink));
    }

  private:
    PIMPL m_pimpl;

  protected:
    Reader(PSOURCE&& _source, PSINK&& _sink);
    
  public:
    template<typename _Sp, typename _Dp>
    explicit Reader(_Sp&& _source, _Dp&& _dataset)
      : Reader(build_source(std::forward<_Sp>(_source)),
	       build_sink(std::forward<_Dp>(_dataset)))  //m_pimpl(build(_source, _dataset))
    {
    }
    ~Reader();

    const Properties& properties() const noexcept;
    void read_all();
    bool read_row();
    size_t current_row_index() const noexcept;
  };
  
}

#endif
