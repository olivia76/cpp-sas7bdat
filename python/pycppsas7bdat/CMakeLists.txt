ADD_SUBDIRECTORY(cpp)

# copy all the py modules to dist folder
file(RELATIVE_PATH DST_DIR ${SRC_DIR} "${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB PYFILES LIST_DIRECTORIES false RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.py")
install(FILES ${PYFILES} DESTINATION ${DST_DIR} COMPONENT "PY") # copy all the py-files in dir

# copy boost dlls
if (WIN32)
    # find all library entries with boost_python37
    list(FILTER Boost_LIBRARIES INCLUDE REGEX "/boost_${BOOST_PYTHON_COMPONENT}-")
    list(TRANSFORM Boost_LIBRARIES REPLACE ".lib$" ${CMAKE_SHARED_LIBRARY_SUFFIX})
    install(FILES ${Boost_LIBRARIES} DESTINATION ${DST_DIR} COMPONENT "EXT") # copy all the library-files in dir
endif()
