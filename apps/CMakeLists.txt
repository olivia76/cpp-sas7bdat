
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(fmt)
find_package(spdlog)
find_package(docopt COMPONENTS CXX REQUIRED)
message (STATUS ${DOCOPT_INCLUDE_DIRS})
include_directories(${DOCOPT_INCLUDE_DIRS})

add_executable(cppsas7bdat-ci
    cppsas7bdat-ci.cpp
    )
target_link_libraries(cppsas7bdat-ci PUBLIC lib_cppsas7bdat)
target_link_libraries(cppsas7bdat-ci PRIVATE fmt::fmt)
target_link_libraries(cppsas7bdat-ci PRIVATE docopt)
target_link_libraries(cppsas7bdat-ci PRIVATE Boost::program_options)
