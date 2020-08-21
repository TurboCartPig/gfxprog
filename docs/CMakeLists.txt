set(DOXYGEN_EXTRACT_ALL ON)
set(DOXYGEN_BUILTIN_STL_SUPPORT ON)

doxygen_add_docs(docs project/lib.h "${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md"
                 WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/include")
