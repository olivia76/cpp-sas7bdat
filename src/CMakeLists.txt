configure_file(
  "${PROJECT_SOURCE_DIR}/include/cppsas7bdat/version.hpp.in"
  "${PROJECT_BINARY_DIR}/cppsas7bdat/version.hpp"
  )

find_package(fmt)
find_package(spdlog)

add_library(cppsas7bdat ${LIBRARY_TYPE}
  buffer.hpp
  column.cpp
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
  types.cpp
  stream.hpp
  stream.cpp
    )
target_include_directories(cppsas7bdat PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(cppsas7bdat PUBLIC ${PROJECT_BINARY_DIR} )
target_link_libraries(cppsas7bdat
  PRIVATE
  fmt::fmt
  spdlog::spdlog
  Boost::date_time 
  project_options
  )
install(TARGETS cppsas7bdat)
