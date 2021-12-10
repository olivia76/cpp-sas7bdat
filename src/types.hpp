/**
 *  \file src/types.hpp
 * 
 *  \brief Managing types
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_SRC_TYPES_HPP_
#define _CPP_SAS7BDAT_SRC_TYPES_HPP_

#include <cstdint>
#include <cstring>
#include <cctype>
#include <type_traits>
#include <boost/endian/conversion.hpp>

namespace cppsas7bdat {
  namespace INTERNAL {

    inline std::string_view get_string(const uint8_t* _buf, const size_t _length) {
      return std::string_view(reinterpret_cast<const char*>(_buf), _length);
    }

    inline std::string_view get_string_trim_0(const uint8_t* _buf, size_t _length) {
      while(_length && (_buf[_length-1] == 0 || std::isspace(_buf[_length-1]))) { --_length; }
      return get_string(_buf, _length);
    }

    inline std::string_view get_string_trim(const uint8_t* _buf, size_t _length) {
      size_t offset = 0;
      while(_length && std::isspace(_buf[offset])) { ++offset; --_length; }
      while(_length && (!std::isprint(_buf[offset+_length-1]) || std::isspace(_buf[offset+_length-1]))) { --_length; }
      return get_string(_buf+offset, _length);
    }

    using BYTE = uint8_t;
    using BYTES = std::basic_string_view<BYTE>;
    inline BYTES get_bytes(const uint8_t* _buf, size_t _length) {
      return BYTES(_buf, _length);
    }

    std::basic_string<BYTE> operator"" _bytes ( const char* _array, const size_t _length )
    {
      auto c2b = [](const char c) {
		   if(c=='0') return 0;
		   if(c=='1') return 1;
		   if(c=='2') return 2;
		   if(c=='3') return 3;
		   if(c=='4') return 4;
		   if(c=='5') return 5;
		   if(c=='6') return 6;
		   if(c=='7') return 7;
		   if(c=='8') return 8;
		   if(c=='9') return 9;
		   if(c=='a' || c=='A') return 10;
		   if(c=='b' || c=='B') return 11;
		   if(c=='c' || c=='C') return 12;
		   if(c=='d' || c=='D') return 13;
		   if(c=='e' || c=='E') return 14;
		   if(c=='f' || c=='F') return 15;
		   throw std::exception();
		 };
      const size_t n = _length/2;
      std::basic_string<BYTE> s(n, 0);
      for(size_t i=0, j=0; i != n; ++i,j+=2) {
	s[i] = c2b(_array[j])*16+c2b(_array[j+1]);
      } 
      return s;
    }

    template<Endian _endian>
    struct ENDIAN {
      template<typename _Tp>
      static inline _Tp get_val(const uint8_t* _buf)
      {
	const _Tp* x = reinterpret_cast<const _Tp*>(_buf);
	if constexpr (_endian == Endian::big) return boost::endian::big_to_native(*x);
	else return boost::endian::little_to_native(*x);
      } 
    };

    template<Endian _endian, typename _Tp>
    inline _Tp get_val(const uint8_t* _buf)
    {
      return ENDIAN<_endian>::template get_val<_Tp>(_buf);
    }
    
    template<Endian _endian>
    inline double get_double(const uint8_t* _buf)
    {
      using IDATA = int64_t;
      using ODATA = double;
      static_assert(sizeof(IDATA) == sizeof(ODATA));
      IDATA x = get_val<_endian, IDATA>(_buf);
      ODATA d;
      //return *(reinterpret_cast<ODATA*>(&x)); // UB: https://en.cppreference.com/w/cpp/language/reinterpret_cast
      std::memcpy(&d, &x, sizeof(d));
      return d;
    }
    
    template<Endian _endian, int _nbits>
    inline double get_incomplete_double(const uint8_t* _buf)
    {
      constexpr int n = sizeof(int64_t);
      alignas(n) uint8_t buf[n];
      if constexpr (_endian == Endian::big) {
	for(int i=0; i<_nbits; ++i) buf[i] = _buf[i];
	for(int i=_nbits; i<n; ++i) buf[i] = 0;
      } else {
	constexpr int nn=n-_nbits;
	for(int i=0; i<nn; ++i) buf[i] = 0;
	for(int i=nn; i<n; ++i) buf[i] = _buf[i-nn];
      }      
      return get_double<_endian>(buf);
    }

    inline DATETIME get_datetime_from_epoch(double _seconds)
    {
      using namespace boost::posix_time;
      using namespace boost::gregorian;
      if(std::isnan(_seconds)) return not_a_date_time;
      ptime start(boost::gregorian::date(1960,1,1));
      // Increase accurary by splitting in days/seconds/microseconds
      constexpr double seconds_in_a_day{24*60*60};
      const auto _days = std::round(_seconds/seconds_in_a_day);
      _seconds -= _days * seconds_in_a_day;
      const auto _secs = std::round(_seconds);
      const auto _microseconds = (_seconds-_secs)*1e6;
      return start + days(_days) + seconds(_secs) + microseconds(_microseconds);
    }

    inline TIME get_time_from_epoch(const double _seconds)
    {
      return get_datetime_from_epoch(_seconds).time_of_day();
    }

    inline DATE get_date_from_epoch_seconds(const double _seconds)
    {
      return get_datetime_from_epoch(_seconds).date();
    }

    inline DATE get_date_from_epoch_days(const double _days)
    {
      using namespace boost::posix_time;
      using namespace boost::gregorian;
      if(std::isnan(_days)) return DATE(not_a_date_time);
      ptime start(boost::gregorian::date(1960,1,1));
      return (start + days(_days)).date();
    }

    inline DATE get_date_from_epoch(const double _t)
    {
      const auto x = get_date_from_epoch_days(_t);
      if(x.is_not_a_date()) return get_date_from_epoch_seconds(_t);
      else return x;
    }
    
    template<Format _format>
    struct number_types {};

    template<>
    struct number_types<Format::bit64> {
      using integer_t = int64_t;
      using uinteger_t = uint64_t;
    };

    template<>
    struct number_types<Format::bit32> {
      using integer_t = int32_t;
      using uinteger_t = uint32_t;

    };
    
  }
}

#endif
