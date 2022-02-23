/**
 *  \file src/memory.hpp
 * 
 *  \brief Memory management for buffer
 *
 *  \author Olivia Quinet
 */

#ifndef _CPP_SAS7BDAT_SRC_MEMORY_HPP_
#define _CPP_SAS7BDAT_SRC_MEMORY_HPP_

#include <new>
#include <memory>
#include <cstdint>
#include <spdlog/spdlog.h>
#include <fmt/core.h>
#include "exceptions.hpp"

namespace cppsas7bdat {
  namespace INTERNAL {
    namespace MEMORY {
      constexpr std::align_val_t al{alignof(uint64_t)};
      inline uint8_t* aligned_alloc(const size_t _size)
      {
	D(spdlog::info(fmt::format("INTERNAL::MEMORY::aligned_alloc({})", _size)));
	void* pv = operator new(_size, al);
	if(!pv) EXCEPTION::cannot_allocate_memory();
	return reinterpret_cast<uint8_t*>(pv);
      }
      inline void aligned_free(uint8_t* _buffer)
      {
	if(_buffer) {
	  void *pv = _buffer;
	  D(spdlog::info(fmt::format("INTERNAL::MEMORY::aligned_free({})", pv)));
	  operator delete(pv, al);
	}
      }

      struct PALIGNEDMEMDeleter {
	void operator()(uint8_t* _buffer) { aligned_free(_buffer); }
      };

      using PALIGNEDMEM = std::unique_ptr<uint8_t, PALIGNEDMEMDeleter>;

    }
  }
}

#endif
