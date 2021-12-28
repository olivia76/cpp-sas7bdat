
find_package(fmt)
find_package(spdlog)
find_package(docopt)
message (STATUS ${DOCOPT_INCLUDE_DIRS})
include_directories(${DOCOPT_INCLUDE_DIRS})

add_executable(cppsas7bdat-ci
    cppsas7bdat-ci.cpp
    )
target_link_libraries(cppsas7bdat-ci PUBLIC lib_cppsas7bdat)
target_link_libraries(cppsas7bdat-ci INTERFACE docopt)
target_link_libraries(cppsas7bdat-ci PRIVATE docopt fmt::fmt)
