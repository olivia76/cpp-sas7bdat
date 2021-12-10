cmake_minimum_required(VERSION 3.15)

#Set the project name
project(
  cppsas7bdat
  VERSION 1.0.0.0
  DESCRIPTION "A C++17 SAS7BDAT reader library"
  LANGUAGES CXX
  )

#include(cmake/StandardProjectSettings.cmake)
#include(cmake/PreventInSourceBuilds.cmake)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(project_options INTERFACE -ftime-trace)
  endif()
endif()

set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_CXX_COMPILER g++)
#set(CMAKE_C_COMPILER gcc)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
#include_directories(include)

set(SRC_LIST
    apps/cppsas7bdat-ci.cpp
    src/sas7bdat.cpp
    src/version.cpp)
set(header_list
    ${PROJECT_BINARY_DIR}/cppsas7bdat/version.hpp
    include/cppsas7bdat/sas7bdat.hpp)

#file(GLOB apps_SRC apps *.cpp *.hpp)
#file(GLOB src_SRC src *.cpp *.hpp)
#file(GLOB include_SRC include *.cpp *.hpp)

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${apps_SRC})
#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_SRC})
#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${include_SRC})
#source_group(src)
#source_group(include)

#set(files
#    ./include/cppsas7bdat/sas7bdat.hpp)
#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${files})

configure_file(
  "${PROJECT_SOURCE_DIR}/include/cppsas7bdat/version.hpp.in"
  "${PROJECT_BINARY_DIR}/cppsas7bdat/version.hpp"
  )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -pedantic -O3)

include(FetchContent)

# Formatting library
FetchContent_Declare(
  fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 5.3.0)
FetchContent_MakeAvailable(fmtlib) # Adds fmt::fmt

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

set(Boost_INCLUDE_DIR /usr/local/boost_1_59_0/include)
set(Boost_LIBRARY_DIR /usr/local/boost_1_59_0/lib)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS program_options filesystem date_time)
message(STATUS "Boost version: ${Boost_VERSION}")
include_directories( ${Boost_INCLUDE_DIR} )

add_subdirectory(src)
add_subdirectory(apps)
#add_subdirectory(tests)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

include(CPack)
