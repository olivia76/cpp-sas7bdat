configure_file(
  "${PROJECT_SOURCE_DIR}/include/cppsas7bdat/version.hpp.in"
  "${PROJECT_BINARY_DIR}/cppsas7bdat/version.hpp"
  )

find_package(Boost REQUIRED COMPONENTS date_time)
find_package(fmt)
find_package(spdlog)

add_library(lib_cppsas7bdat
    exceptions.hpp
    exceptions.cpp
    memory.hpp
    types.hpp
    stream.hpp
    header.hpp
    metadata.hpp
    formatters.hpp
    buffer.hpp
    debug.hpp
    data.hpp
    page.hpp
    sas7bdat.cpp
    sas7bdat-impl.hpp
    version.cpp
    datasource_ifstream.cpp
    )
target_include_directories(lib_cppsas7bdat PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(lib_cppsas7bdat PUBLIC ${PROJECT_BINARY_DIR} )
target_link_libraries(lib_cppsas7bdat
  PRIVATE
  fmt::fmt
  spdlog::spdlog_header_only
  Boost::date_time 
  )
