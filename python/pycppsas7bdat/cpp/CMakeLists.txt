find_package(fmt)

get_filename_component(TARGET ${CMAKE_CURRENT_SOURCE_DIR} NAME)
Python3_add_library(${TARGET} MODULE pycppsas7bdat.cpp import_datetime.cpp import_datetime.hpp reader.cpp reader.hpp sink.hpp)
set_target_properties(${TARGET} PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}")
set_target_properties(${TARGET} PROPERTIES SUFFIX "${PYTHON_MODULE_EXTENSION}")

target_link_libraries(${TARGET} PRIVATE ${Boost_LIBRARIES})
target_link_libraries(${TARGET} PRIVATE fmt::fmt)
target_link_libraries(${TARGET} PRIVATE Python3::NumPy)
target_link_libraries(${TARGET} PRIVATE ${BOOST_PYTHON_LIBRARY})
target_link_libraries(${TARGET} PRIVATE cppsas7bdat)

# install pyd files one directory structure UP in the destination as
# CMAKE_CURRENT_SOURCE_DIR is holding the source files of the MODULE
# which matches the name of CMAKE_CURRENT_SOURCE_DIR
get_filename_component(CURRENT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
if(${SRC_DIR} STREQUAL ${CURRENT_SRC_DIR})
    set(DST_DIR ".") # if parent is the base folder
else()
    file(RELATIVE_PATH DST_DIR ${SRC_DIR} ${CURRENT_SRC_DIR})
endif()
#install(TARGETS ${TARGET} RUNTIME DESTINATION ${DST_DIR} COMPONENT "EXT")
install(TARGETS ${TARGET} LIBRARY DESTINATION ${DST_DIR} COMPONENT "EXT")
