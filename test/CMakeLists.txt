find_package(Catch2 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog)

include(CTest)
include(Catch)

add_library(catch_main STATIC catch_main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)
target_link_libraries(catch_main PRIVATE project_options)

add_executable(tests tests_internal.cpp tests_interface.cpp tests_decompressors.cpp tests_exceptions.cpp tests_formatters.cpp tests_version.cpp tests_column.cpp tests_column-filter.cpp)
target_link_libraries(tests
  PRIVATE
  catch_main
  nlohmann_json::nlohmann_json
  spdlog::spdlog
  Boost::date_time
  Boost::filesystem
  cppsas7bdat
  project_options
  )
#  project_warnings
#  

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
