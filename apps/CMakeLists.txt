add_executable(cppsas7bdat-ci
    cppsas7bdat-ci.cpp
    )
target_link_libraries(cppsas7bdat-ci PRIVATE lib_cppsas7bdat fmt::fmt Boost::program_options Boost::filesystem)
