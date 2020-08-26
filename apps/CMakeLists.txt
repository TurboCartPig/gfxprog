add_executable(triangle triangle.cpp)
target_compile_options(triangle PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(triangle PRIVATE lib GLEW::GLEW glfw glm OpenGL::GL)
target_precompile_headers(triangle PRIVATE <iostream> <string> <vector>)
