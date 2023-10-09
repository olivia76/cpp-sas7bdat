find_package(nlohmann_json REQUIRED)
find_package(spdlog)

#find_package(Catch2 3 REQUIRED)
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)

include(CTest)
include(Catch)

#add_library(catch_main STATIC catch_main.cpp)
#target_link_libraries(catch_main PUBLIC Catch2::Catch2)
#target_link_libraries(catch_main PRIVATE project_options)

add_executable(tests
  tests_reader.cpp
  tests_encodings.cpp
  tests_internal.cpp
  tests_interface.cpp
  tests_decompressors.cpp
  tests_exceptions.cpp
  tests_formatters.cpp
  tests_version.cpp
  tests_column.cpp
  tests_column-filter.cpp
  tests_types.cpp
  tests_memory.cpp
  )

target_link_libraries(tests
  PRIVATE
  Catch2::Catch2WithMain
  #catch_main
  nlohmann_json::nlohmann_json
  spdlog::spdlog
  Boost::date_time
  cppsas7bdat
  project_options
  )
#  project_warnings
#

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/reports)
catch_discover_tests(
  tests
  OUTPUT_DIR ${PROJECT_SOURCE_DIR}/reports
  REPORTER junit
  OUTPUT_PREFIX "unittests."
  OUTPUT_SUFFIX .xml
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
