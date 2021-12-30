find_package(Catch2 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog)

include(CTest)
include(Catch)

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)
#target_link_libraries(catch_main PRIVATE project_options)

add_executable(tests tests_internal.cpp)
target_link_libraries(tests
  PRIVATE
  catch_main
  nlohmann_json::nlohmann_json
  spdlog::spdlog
  lib_cppsas7bdat
  )
#  project_warnings
#  project_options

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  tests
  OUTPUT_DIR ${PROJECT_SOURCE_DIR}/reports
  REPORTER junit
  OUTPUT_PREFIX "unittests."
  OUTPUT_SUFFIX .xml
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

#include(CTest)
#include(Catch)

#set (WILDCARD_SOURCE *.cpp)
#file(GLOB_RECURSE TEST_SOURCES ${WILDCARD_SOURCE})

#add_library(catch_main STATIC catch_main.cpp)
#target_link_libraries(catch_main PUBLIC Catch2::Catch2)

#add_executable(tests test_internal.cpp)

#target_include_directories(tests
#  PRIVATE
#  ${CMAKE_SOURCE_DIR}/include
#  )
#target_link_libraries(tests
#  PRIVATE
#  catch_main
#  Catch2::Catch2
#  fmt::fmt
#  nlohmann_json::nlohmann_json
#  lib_cppsas7bdat
#  )


#catch_discover_tests(tests
#  OUTPUT_DIR ${PROJECT_SOURCE_DIR}/reports
#  REPORTER junit
#  OUTPUT_SUFFIX .xml
#  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#  )

#if (CATCH_ENABLE_COVERAGE)
#    set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
#    find_package(codecov)
#    add_coverage(SelfTest)
#    list(APPEND LCOV_REMOVE_PATTERNS "'/usr/*'")
#    coverage_evaluate()
#endif()
