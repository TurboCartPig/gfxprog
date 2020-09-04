add_library(lib lib.cpp ShaderProgram.cpp VertexBufferObject.cpp Window.cpp)
target_compile_options(lib PRIVATE ${COMPILER_WARNINGS_AND_ERRORS})
target_include_directories(lib PUBLIC ../include)
target_link_libraries(lib PUBLIC GLEW::GLEW glfw glm OpenGL::GL)
