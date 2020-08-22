cmake_minimum_required(VERSION 3.16...3.18)
project(
  Project
  VERSION 0.1.0
  DESCRIPTION "A CMake template project"
  LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Set global preferences
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
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

add_subdirectory(lib)
add_subdirectory(apps)
