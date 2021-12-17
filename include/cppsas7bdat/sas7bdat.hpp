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
    struct DatasetSinkConcept {
      class Internal;
      using PINTERNAL = std::unique_ptr<Internal>;
      PINTERNAL pinternal;
      
      explicit DatasetSinkConcept(const char* _pcszFileName);
      virtual ~DatasetSinkConcept();
      
      const Properties& properties() const noexcept;
      const COLUMNS& columns() const noexcept;
      size_t current_row_index() const noexcept;

      void read_all();
      bool read_row();

      virtual void set_properties() = 0;
      virtual void read_row(Column::PBUF _p) = 0;
    };

    template<typename _Dp>
    struct DatasetSinkModel : DatasetSinkConcept {
      DatasetSinkModel(const char* _pcszFileName, _Dp&& _dataset)
	: DatasetSinkConcept(_pcszFileName),
	  dataset(std::forward<_Dp>(_dataset)) {}

      void set_properties() final
      {
	dataset.set_properties(properties());
      }
      
      void read_row(Column::PBUF _p) final
      {
	dataset.read_row(current_row_index(), _p);
      }
      
    private:
      _Dp dataset;
    };
    using PIMPL = std::unique_ptr<DatasetSinkConcept>;
    PIMPL m_pimpl;
   
    template<typename _Dp>
    static PIMPL build(const char* _pcszFileName, _Dp&& _datasink)
    {
      return std::make_unique< DatasetSinkModel<_Dp> >(_pcszFileName, std::forward<_Dp>(_datasink));
    }
    
  public:
    template<typename _Dp>
    explicit Reader(const char* _pcszFileName, _Dp&& _dataset)
      : m_pimpl(build(_pcszFileName, _dataset))
    {
      m_pimpl->set_properties();
    }
    ~Reader();

    const Properties& properties() const noexcept { return m_pimpl->properties(); }

    void read_all() { m_pimpl->read_all(); }
    bool read_row() { return m_pimpl->read_row(); }
    size_t current_row_index() const noexcept { return m_pimpl->current_row_index(); }
  };
  
}

#endif
