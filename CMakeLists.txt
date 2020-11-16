cmake_minimum_required(VERSION 3.16...3.18)

# Needed for assimp on linux
cmake_policy(SET CMP0012 OLD)

# Pass cmake "-DNO_VCPKG=ON" to disable usage of vcpkg
if(NOT ${NO_VCPKG})
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

project(
    gfxprog
    VERSION 0.1.0
    DESCRIPTION
        "Labs and assignments completed in association with Graphics Programming / prog2002 at NTNU"
    LANGUAGES CXX)

include(CTest)
include(FetchContent)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# List warnings and errors to be used by internal targets
set(COMPILER_WARNINGS_AND_ERRORS
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/w44061>
    $<$<CXX_COMPILER_ID:MSVC>:/w44062>
    $<$<CXX_COMPILER_ID:MSVC>:/w44265>
    $<$<CXX_COMPILER_ID:MSVC>:/w44267>
    $<$<CXX_COMPILER_ID:MSVC>:/permissive->
    $<$<CXX_COMPILER_ID:GNU>:-Wmisleading-indentation>
    $<$<CXX_COMPILER_ID:GNU>:-Wduplicated-cond>
    $<$<CXX_COMPILER_ID:GNU>:-Wduplicated-branches>
    $<$<CXX_COMPILER_ID:GNU>:-Wlogical-op>
    $<$<CXX_COMPILER_ID:GNU>:-Wnull-dereference>
    $<$<CXX_COMPILER_ID:GNU>:-Wuseless-cast>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wdocumentation>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wweak-vtables>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wexit-time-destructors>
    $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wglobal-constructors>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wall>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wextra>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wpedantic>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wshadow>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wnon-virtual-dtor>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Woverloaded-virtual>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wunreachable-code>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wmissing-noreturn>)

set(COMPILER_SANITIZERS
    # FIXME: Need to link to some llvm libraries that are installed alongside
    # visual studio
    # $<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address>
    # FIXME: DIfferent linking erros
    # $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-fsanitize=address,undefined>)
)

# Dependencies
# ##############################################################################
# Find or download dependencies

find_package(OpenGL REQUIRED)

find_package(glfw3 3.3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Assimp CONFIG REQUIRED)
find_package(GLEW 2.1 MODULE REQUIRED)
find_package(Stb MODULE REQUIRED)

# Fixes inconsistency in how vcpkg and native package managers exports assimp
if(NOT TARGET assimp::assimp)
    add_library(assimp::assimp INTERFACE IMPORTED)
    target_link_libraries(assimp::assimp INTERFACE assimp)
endif()

# Subdirectories defining actual targets
# ##############################################################################

# Things to do only if current project is top level project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Setup compile_commands for clangd TODO: This is a hack and I should find a
    # better way to do this
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    execute_process(
        COMMAND
            ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Enable testing
    if(BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()

add_subdirectory(lib)
add_subdirectory(apps)
add_subdirectory(docs)
