set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

doxygen_add_docs(docs glove/lib.h glove/ShaderProgram.h glove/VertexBufferObject.h glove/VertexFormats.h glove/Window.h "${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/include")
