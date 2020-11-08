# Glob headers and source files Earlier versions of cmake did not handle this
# very well, but since we 'cmake_minimum_required' a recent version, this is now
# fine
file(GLOB GLOVE_HEADER_FILES CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/include/glove/*.h")
file(GLOB GLOVE_SOURCE_FILES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(lib ${GLOVE_SOURCE_FILES})
target_compile_options(lib PRIVATE ${COMPILER_WARNINGS_AND_ERRORS}
                                   ${COMPILER_SANITIZERS})
target_compile_definitions(lib PUBLIC _USE_MATH_DEFINES)
target_include_directories(lib PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(
    lib
    PUBLIC glm GLEW::GLEW glfw
    PRIVATE OpenGL::GL Stb::Stb assimp::assimp)
target_precompile_headers(
    lib
    PRIVATE
    <string>
    <vector>
    <array>
    <unordered_map>
    <iostream>
    <chrono>
    <memory>
    <utility>
    <functional>
    <GL/glew.h>
    <glm/glm.hpp>
    <glm/gtc/type_ptr.hpp>
    <glm/gtc/matrix_transform.hpp>)

set_target_properties(
    lib
    PROPERTIES CXX_STANDARD 17
               CXX_STANDARD_REQUIRED ON
               CXX_EXTENSIONS OFF
               MSVC_RUNTIME_LIBRARY "MultiThreadedDebug"
               FOLDER "Glove")

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include/glove"
    PREFIX "Header Files"
    FILES ${GLOVE_HEADER_FILES})

source_group(
    TREE "${PROJECT_SOURCE_DIR}/lib"
    PREFIX "Source Files"
    FILES ${GLOVE_SOURCE_FILES})
