find_package(Doxygen QUIET)

if (Doxygen_FOUND)
    message(STATUS "Enabling docs")
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

    file(GLOB GLOVE_HEADER_FILES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/glove/*.h")

    doxygen_add_docs(docs ${GLOVE_HEADER_FILES} "${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md"
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endif ()
