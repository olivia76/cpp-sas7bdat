configure_file(
  "${PROJECT_SOURCE_DIR}/include/cppsas7bdat/version.hpp.in"
  "${PROJECT_BINARY_DIR}/cppsas7bdat/version.hpp"
  )

find_package(Boost REQUIRED COMPONENTS date_time)
find_package(fmt)
find_package(spdlog)

add_library(lib_cppsas7bdat
  buffer.hpp
  datasource_ifstream.cpp
  decompressors.hpp
  exceptions.hpp
  header.hpp
  metadata.hpp
  sas7bdat.cpp
  stream.hpp
  types.hpp
  data.hpp
  debug.hpp
  exceptions.cpp
  formatters.hpp
  memory.hpp
  page.hpp
  sas7bdat-impl.hpp
  subheaders.hpp
  version.cpp
    )
target_include_directories(lib_cppsas7bdat PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(lib_cppsas7bdat PUBLIC ${PROJECT_BINARY_DIR} )
target_link_libraries(lib_cppsas7bdat
  PRIVATE
  fmt::fmt
  spdlog::spdlog
  Boost::date_time 
  PRIVATE project_options
  )
