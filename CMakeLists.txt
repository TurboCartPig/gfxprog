cmake_minimum_required(VERSION 3.16...3.18)
project(
        Project
        VERSION 0.1.0
        DESCRIPTION "A CMake template project"
        LANGUAGES CXX)

include(CTest)
include(FetchContent)

# List warnings and errors to be used by internal targets
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
        $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wglobal-constructors>)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Setup compile_commands for clangd
    # TODO: This is a hack and I should find a better way to do this
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    execute_process(
            COMMAND
            ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json)

    # Enable testing
    if (BUILD_TESTING)
        add_subdirectory(tests)
    endif ()

    # Enable docs
    find_package(Doxygen)
    if (Doxygen_FOUND)
        message(STATUS "Enabling docs")
        add_subdirectory(docs)
    endif ()
endif ()

# Dependencies
##############################################################################
# Find or download dependencies

find_package(OpenGL REQUIRED)
find_package(GLEW 2.1 QUIET)
find_package(glfw3 3.3 QUIET)
find_package(glm QUIET)
find_package(imgui QUIET)

# Try to find stb installation locally first, and download it from github as a fallback.
# Stb is a header only library, so all we need to do is find the directory in which it resides
find_path(
        STB_INCLUDE_DIRS
        NAMES "stb.h" "stb_image.h"
        HINTS "/usr/include/")

# Download stb from github, if not found locally
if (STB_INCLUDE_DIRS MATCHES STB_INCLUDE_DIRS-NOTFOUND)
    message(STATUS "Stb not found, downloading...")

    FetchContent_Declare(
            stb
            GIT_REPOSITORY https://github.com/nothings/stb
            GIT_TAG master)

    FetchContent_GetProperties(stb)
    if (NOT stb_POPULATED)
        FetchContent_Populate(stb)

        # Define stb "linkable" target
        add_library(stb INTERFACE)
        target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
    endif ()
else ()
    message(STATUS "Stb found, using: ${STB_INCLUDE_DIRS}")

    # Define stb "linkable" target
    add_library(stb INTERFACE)
    target_include_directories(stb INTERFACE ${STB_INCLUDE_DIRS})
endif ()

if (NOT imgui_FOUND)
    message(STATUS "ImGui not found locally, downloading...")
    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui
            GIT_TAG v1.78)
    FetchContent_MakeAvailable(imgui)
endif ()

if (NOT glfw3_FOUND)
    message(STATUS "GLFW3 not found locally, downloading...")
    FetchContent_Declare(
            glfw3
            GIT_REPOSITORY https://github.com/glfw/glfw
            GIT_TAG 3.3.2)
    # Disable additional targets because the names of imported target collide with
    # locally defined targets
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_DOCS OFF)
    set(GLFW_BUILD_TESTS OFF)
    FetchContent_MakeAvailable(glfw3)
endif ()

if (NOT GLEW_FOUND)
    message(STATUS "GLEW not found locally, downloading...")
    FetchContent_Declare(
            GLEW
            GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
            GIT_TAG glew-cmake-2.1.0)
    FetchContent_MakeAvailable(GLEW)

    # Add alias so that the target imported from find_package and this targets
    # have the same name
    add_library(GLEW::GLEW ALIAS libglew_static)
endif ()

if (NOT glm_FOUND)
    message(STATUS "GLM not found locally, downloading...")
    FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm
            GIT_TAG 0.9.9.8)
    FetchContent_MakeAvailable(glm)
endif ()

# Subdirectories defining actual targets
##############################################################################

add_subdirectory(lib)
add_subdirectory(apps)
