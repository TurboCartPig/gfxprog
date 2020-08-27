cmake_minimum_required(VERSION 3.16...3.18)
project(
  Project
  VERSION 0.1.0
  DESCRIPTION "A CMake template project"
  LANGUAGES CXX)

# List warnings and errors to be used by internal tergets
set(COMPILER_WARNINGS_AND_ERRORS
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/w44061>
    $<$<CXX_COMPILER_ID:MSVC>:/w44062>
    $<$<CXX_COMPILER_ID:MSVC>:/w44265>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wall>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wextra>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wpedantic>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wunreachable-code>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wmissing-noreturn>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wdocumentation>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wweak-vtables>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wexit-time-destructors>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wglobal-constructors>
#    $<$<AND:$<CXX_COMPILER_ID:Clang,AppleClang,GNU>,$<CONFIG:DEBUG>>:-fsanitize=undefined,address> # FIXME: Doesn't work right now
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Set global preferences
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Enable LTO globally
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

  # Setup compile_commands for clangd
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
  execute_process(
    COMMAND
      ${CMAKE_COMMAND} -E create_symlink
      ${CMAKE_BINARY_DIR}/compile_commands.json
      ${CMAKE_SOURCE_DIR}/compile_commands.json)

  # Enable testing
  include(CTest)
  if(BUILD_TESTING)
    add_subdirectory(tests)
  endif()

  # Enable docs
  find_package(Doxygen)
  if(Doxygen_FOUND)
    message("-- Enabling docs")
    add_subdirectory(docs)
  endif()
endif()

# Dependencies
find_package(OpenGL REQUIRED)
find_package(GLEW 2.1 REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)

add_subdirectory(lib)
add_subdirectory(apps)
