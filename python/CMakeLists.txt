cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)  #NumPy

# get the suffix for python extension module and store in PYTHON_MODULE_EXTENSION
execute_process (COMMAND "${Python3_EXECUTABLE}" -c "from distutils import sysconfig;print(sysconfig.get_config_var('EXT_SUFFIX'))"
RESULT_VARIABLE _result
OUTPUT_VARIABLE PYTHON_MODULE_EXTENSION
ERROR_QUIET
OUTPUT_STRIP_TRAILING_WHITESPACE)

set(BOOST_PYTHON_COMPONENT "python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR}")
#set(BOOST_PYTHON_LIBRARY "Boost::${BOOST_PYTHON_COMPONENT}")
set(BOOST_PYTHON_LIBRARY "boost::${BOOST_PYTHON_COMPONENT}")  # Beware...!!!

message(STATUS "BOOST_PYTHON_COMPONENT: ${BOOST_PYTHON_COMPONENT}")
message(STATUS "BOOST_PYTHON_LIBRARY: ${BOOST_PYTHON_LIBRARY}")

find_package(Boost REQUIRED COMPONENTS ${BOOST_PYTHON_COMPONENT})


set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("pycppsas7bdat")

#python3_add_library(pycppsas7bdat MODULE main.cpp)

#target_link_libraries(pycppsas7bdat PRIVATE ${Boost_LIBRARIES})
#target_link_libraries(pycppsas7bdat PRIVATE cppsas7bdat)
#target_link_libraries(pycppsas7bdat PRIVATE ${Python3_LIBRARIES} Python3::NumPy)
#target_include_directories(pycppsas7bdat PRIVATE  ${Python3_INCLUDE_DIRS})

#configure_file(__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)
#set_target_properties(pycppsas7bdat PROPERTIES PREFIX "")  # no 'lib' prefix
#set_target_properties(pycppsas7bdat PROPERTIES SUFFIX ".so")  # must be .so (not .dylib)

##PYTHON_ADD_MODULE(pycppsas7bdat main)
#FILE(COPY cppsas7bdat.py DESTINATION .)
