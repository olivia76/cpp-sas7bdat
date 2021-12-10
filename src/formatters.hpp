/**
 *  \file src/formatters.hpp
 * 
 *  \brief Column Value Formatters
 *
 *  \author Olivia Quinet
 */


#ifndef _CPP_SAS7BDAT_SRC_FORMATTERS_HPP_
#define _CPP_SAS7BDAT_SRC_FORMATTERS_HPP_

#include <limits>

namespace cppsas7bdat {
  namespace INTERNAL {
    namespace FORMATTER {

      using Type = Column::Type;

      struct IFormatter {
	const size_t offset{0};
	const size_t length{0};
	const Type type{Type::unknown};

	IFormatter(const size_t _offset, const size_t _length, const Type _type)
	  : offset(_offset),
	    length(_length),
	    type(_type)
	{
	  //fmt::print(stderr, "IFormatter: {}:{} ", offset, length);
	}

	STRING   get_string  ([[maybe_unused]] const void* _p) const noexcept { return {}; }
	NUMBER   get_number  ([[maybe_unused]] const void* _p) const noexcept { return std::numeric_limits<NUMBER>::quiet_NaN(); }
	INTEGER  get_integer ([[maybe_unused]] const void* _p) const noexcept { return {}; }
	DATETIME get_datetime([[maybe_unused]] const void* _p) const noexcept { return {}; }
	DATE     get_date    ([[maybe_unused]] const void* _p) const noexcept { return {}; }
	TIME     get_time    ([[maybe_unused]] const void* _p) const noexcept { return {}; }

	const uint8_t* data(const void* _p) const noexcept
	{
	  const uint8_t* p = reinterpret_cast<const uint8_t*>(_p) + offset;
	  //fmt::print(stderr, "IFormatter::data: {}, {}, {}\n", _p, offset, (void*)p);	
	  return p;
	}
      };

      struct NoFormatter : public IFormatter {
	NoFormatter(const size_t _offset, const size_t _length) : IFormatter(_offset, _length, Type::unknown) { D(fmt::print(stderr, "NoFormatter: {}:{}\n", offset, length)); }
      };
      
      struct StringFormatter : public IFormatter {
	StringFormatter(const size_t _offset, const size_t _length) : IFormatter(_offset, _length, Type::string) { D(fmt::print(stderr, "StringFormatter\n")); }
	
	STRING get_string(const void* _p) const noexcept
	{
	  return INTERNAL::get_string_trim_0(data(_p), length);
	}
      };
      
      struct SmallIntegerFormatter : public IFormatter {
	SmallIntegerFormatter(const size_t _offset, const size_t _length) : IFormatter(_offset, _length, Type::integer) { D(fmt::print(stderr, "SmallIntegerFormatter\n")); }

	INTEGER get_integer(const void* _p) const noexcept
	{
	  return *data(_p);
	}
	
	NUMBER get_number(const void* _p) const noexcept
	{
	  return get_integer(_p);
	}
	
      };
      
      template<Endian _endian, typename _Tp>
      struct IntegerFormatter : public IFormatter {
	IntegerFormatter(const size_t _offset, const size_t _length) : IFormatter(_offset, _length, Type::integer) { D(fmt::print(stderr, "IntegerFormatter\n")); }

	INTEGER get_integer(const void* _p) const noexcept
	{
	  return INTERNAL::get_val<_endian, _Tp>(data(_p));
	}
	
	NUMBER get_number(const void* _p) const noexcept
	{
	  return get_integer(_p);
	}
	
      };

      template<Endian _endian>
      struct DoubleFormatter : public IFormatter {
	DoubleFormatter(const size_t _offset, const size_t _length, const Type _type=Type::number) : IFormatter(_offset, _length, _type) { D(fmt::print(stderr, "DoubleFormatter\n")); }

	NUMBER get_number([[maybe_unused]] const void* _p) const noexcept
	{
	  //fmt::print(stderr, "DoubleFormatter::get_number: {}\n", _p);		     
	  return INTERNAL::get_double<_endian>(data(_p));
	}
	
      };

      template<Endian _endian, int _nbits>
      struct IncompleteDoubleFormatter : public IFormatter {
	IncompleteDoubleFormatter(const size_t _offset, const size_t _length) : IFormatter(_offset, _length, Type::number) { D(fmt::print(stderr, "IncompleteDoubleFormatter\n")); }

	NUMBER get_number([[maybe_unused]] const void* _p) const noexcept
	{
	  return INTERNAL::get_incomplete_double<_endian, _nbits>(data(_p));
	}
	
      };

      template<Endian _endian>
      struct DateTimeFormatter : public DoubleFormatter<_endian> {
	DateTimeFormatter(const size_t _offset, const size_t _length) : DoubleFormatter<_endian>(_offset, _length, Type::datetime) { D(fmt::print(stderr, "DateTimeFormatter\n")); }

	DATETIME get_datetime([[maybe_unused]] const void* _p) const noexcept
	{
	  return INTERNAL::get_datetime_from_epoch(DoubleFormatter<_endian>::get_number(_p));
	}

	DATE get_date([[maybe_unused]] const void* _p) const noexcept
	{
	  return get_datetime(_p).date();
	}
	
	TIME get_time([[maybe_unused]] const void* _p) const noexcept
	{
	  return get_datetime(_p).time_of_day();
	}
	
      };

      template<Endian _endian>
      struct DateFormatter : public DoubleFormatter<_endian> {
	DateFormatter(const size_t _offset, const size_t _length) : DoubleFormatter<_endian>(_offset, _length, Type::date) { D(fmt::print(stderr, "DateTimeFormatter\n")); }

	DATE get_date([[maybe_unused]] const void* _p) const noexcept
	{
	  return INTERNAL::get_date_from_epoch(DoubleFormatter<_endian>::get_number(_p));
	}

	DATETIME get_datetime([[maybe_unused]] const void* _p) const noexcept
	{
	  return DATETIME(get_date(_p), {});
	}
	
      };

      template<Endian _endian>
      struct TimeFormatter : public DoubleFormatter<_endian> {
	TimeFormatter(const size_t _offset, const size_t _length) : DoubleFormatter<_endian>(_offset, _length, Type::time) { D(fmt::print(stderr, "DateTimeFormatter\n")); }

	TIME get_time([[maybe_unused]] const void* _p) const noexcept
	{
	  return INTERNAL::get_time_from_epoch(DoubleFormatter<_endian>::get_number(_p));
	}

	DATETIME get_datetime([[maybe_unused]] const void* _p) const noexcept
	{
	  return DATETIME({}, get_time(_p));
	}
	
      };
    
    }
  }
}

#endif
