# Glob headers and source files
# Earlier versions of cmake did not handle this very well, but since we 'cmake_minimum_required' a recent version, this is now fine
file(GLOB GLOVE_HEADER_FILES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/glove/*.h")
file(GLOB GLOVE_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(lib ${GLOVE_SOURCE_FILES} ${GLOVE_HEADER_FILES})
target_compile_options(lib PRIVATE ${COMPILER_WARNINGS_AND_ERRORS})
target_compile_definitions(lib PUBLIC _USE_MATH_DEFINES)
target_include_directories(lib PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(lib PUBLIC GLEW::GLEW glfw glm OpenGL::GL stb)
target_precompile_headers(lib PRIVATE <iostream> <chrono> <string> <vector> <array> <GL/glew.h> <glm/glm.hpp> <glm/gtc/type_ptr.hpp> <glm/gtc/matrix_transform.hpp>)
