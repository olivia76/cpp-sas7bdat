# Make a .so output!
#if(APPLE)
#set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
#endif(APPLE)

find_package(PythonInterp)
find_package(PythonLibs) # 3.8 REQUIRED)
string(REPLACE "." ";" VERSION_LIST ${PYTHONLIBS_VERSION_STRING})
list(GET VERSION_LIST 0 PYTHONLIBS_VERSION_MAJOR)                                                                                                                             
list(GET VERSION_LIST 1 PYTHONLIBS_VERSION_MINOR)
#find_package(Boost COMPONENTS python${PYTHONLIBS_VERSION_MAJOR}${PYTHONLIBS_VERSION_MINOR} REQUIRED)
find_package(Boost COMPONENTS python REQUIRED)

add_library(pycppsas7bdat MODULE main.cpp)

target_link_libraries(pycppsas7bdat PRIVATE ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_link_libraries(pycppsas7bdat PRIVATE lib_cppsas7bdat)
target_include_directories(pycppsas7bdat PRIVATE ${PYTHON_INCLUDE_DIRS})

configure_file(__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)
set_target_properties(pycppsas7bdat PROPERTIES PREFIX "")  # no 'lib' prefix
set_target_properties(pycppsas7bdat PROPERTIES SUFFIX ".so")  # must be .so (not .dylib)

#PYTHON_ADD_MODULE(pycppsas7bdat main)
FILE(COPY cppsas7bdat.py DESTINATION .)
