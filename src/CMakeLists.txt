configure_file(
  "${PROJECT_SOURCE_DIR}/include/cppsas7bdat/version.hpp.in"
  "${PROJECT_BINARY_DIR}/cppsas7bdat/version.hpp"
  )

find_package(fmt)
find_package(spdlog)
find_package(Boost REQUIRED COMPONENTS date_time)
message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "BOOST LIBRARIES LOCATION: " ${Boost_LIBRARIES})
#include_directories( ${Boost_INCLUDE_DIR} )

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

target_compile_definitions(cppsas7bdat PRIVATE BOOST_DATE_TIME_NO_LIB)

target_include_directories(cppsas7bdat INTERFACE $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_include_directories(cppsas7bdat INTERFACE ${PROJECT_BINARY_DIR} )
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
