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
  encodings.cpp
  encodings.hpp
  exceptions.hpp
  header.hpp
  metadata.hpp
  reader.cpp
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
target_include_directories(cppsas7bdat PUBLIC $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_include_directories(cppsas7bdat PUBLIC ${PROJECT_BINARY_DIR} )
target_link_libraries(cppsas7bdat
  INTERFACE
  Boost::date_time
  PRIVATE
  fmt::fmt
  spdlog::spdlog
  Boost::date_time
  project_options
  project_warnings
  )

include(GNUInstallDirs)

install(TARGETS cppsas7bdat
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  COMPONENT Devel
  FILES_MATCHING PATTERN "*.hpp"
  )
install(
  FILES "${PROJECT_BINARY_DIR}/cppsas7bdat/version.hpp"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cppsas7bdat
  COMPONENT Devel
  )

if( ipo_supported )
    set_property(TARGET cppsas7bdat PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
