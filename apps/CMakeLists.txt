
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(fmt)
find_package(spdlog)

add_executable(cppsas7bdat-ci
    cppsas7bdat-ci.cpp
    )
target_link_libraries(cppsas7bdat-ci INTERFACE lib_cppsas7bdat)
target_link_libraries(cppsas7bdat-ci PRIVATE lib_cppsas7bdat fmt::fmt Boost::program_options)
