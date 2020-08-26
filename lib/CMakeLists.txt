add_library(lib lib.cpp)
target_compile_options(lib PRIVATE ${COMPILER_WARNINGS_AND_ERRORS})
target_include_directories(lib PUBLIC ../include)
target_link_libraries(lib PRIVATE GLEW::GLEW glfw glm OpenGL::GL)
