/**
 *  \file tests/data.hpp
 *
 *  \brief  
 *
 *  \author  Olivia Quinet
 */

#include <nlohmann/json.hpp>

constexpr const char* invalid_path = "data/invalid_path.sas7bdat";
constexpr const char* file_too_short = "data/file_too_short.err";
constexpr const char* invalid_magic_number = "data/invalid_magic_number.err";
constexpr const char* file1 = "data/file1.sas7bdat";

using json = nlohmann::json;

namespace {
  struct FILES {
    explicit FILES(const char* _filename)
    {
      std::ifstream is(_filename);
      if(!is) throw std::runtime_error("Cannot read filename in test");
      is >> j;
    }
    json j; 
  };
  
  static const FILES& files() {
    static const FILES instance("files.json");
    return instance;
  }
  
  using namespace cppsas7bdat;
  
  auto get_string = [](auto j) {
		      return j.is_null() ? std::string() : std::string(j);
		    };
  auto get_platform = [](const auto& _platform){
			if(_platform.is_null()) return cppsas7bdat::Platform::unknown;
			return _platform == "unix" ? cppsas7bdat::Platform::unix : cppsas7bdat::Platform::windows;
		      };
  auto get_datetime = [](const auto& _x) -> boost::posix_time::ptime {
			using namespace boost::posix_time;
			if(_x.is_null()) return not_a_date_time;
			std::string date = _x;
			const auto ts = time_from_string(date);
			return ts;
		      };

  auto get_dt = [](const boost::posix_time::ptime ts) -> boost::posix_time::ptime {
		  using namespace boost::posix_time;
		  const auto d = ts.date();
		  const auto tod = ts.time_of_day();
		  return ptime(d, time_duration(tod.hours(), tod.minutes(), tod.seconds(), 0));
		};
  
  auto get_date = [](const auto& _date)  -> boost::gregorian::date {
		    using namespace boost::gregorian;
		    if(_date.is_null()) return date(not_a_date_time);
		    const auto d = from_string(_date);
		    return d;
		  };
  auto get_time = [](const auto& _date) -> boost::posix_time::time_duration {
		    using namespace boost::posix_time;
		    if(_date.is_null()) return not_a_date_time;
		    const auto t = duration_from_string(_date);
		    return t;
		  };  
  auto get_compression = [](const auto& _compression) -> cppsas7bdat::Compression {
			   if(_compression.is_null()) return cppsas7bdat::Compression::none;
			   return ( _compression == "SASYZCR2" ? cppsas7bdat::Compression::RDC :
				    _compression == "SASYZCRL" ? cppsas7bdat::Compression::RLE : cppsas7bdat::Compression::none );
			 };
  auto get_column_type = [](const auto& _type) -> cppsas7bdat::Column::Type {
			   if(_type.is_null()) return cppsas7bdat::Column::Type::unknown;
			   return ( _type == "string" ? cppsas7bdat::Column::Type::string :
				    _type == "number" ? cppsas7bdat::Column::Type::number : cppsas7bdat::Column::Type::unknown );
			 };
  auto get_format = [](const bool _u64) -> cppsas7bdat::Format {
		      return _u64 ? cppsas7bdat::Format::bit64 : cppsas7bdat::Format::bit32; 
		    };
  
  auto get_endianness = [](const std::string& _endianness){
			  return _endianness == "little" ? cppsas7bdat::Endian::little : cppsas7bdat::Endian::big;
			};
}
