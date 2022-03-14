cmake_minimum_required(VERSION 3.15)

add_library(Rcppsas7bdat SHARED
  Rcppsas7bdat.cpp
  version.cpp
  version.hpp
  reader.cpp
  reader.hpp
  filter.cpp
  filter.hpp
  )

target_link_libraries(Rcppsas7bdat PRIVATE cppsas7bdat)

