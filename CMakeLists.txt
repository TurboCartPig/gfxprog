cmake_minimum_required(VERSION 3.16...3.18)
project(
        gfxprog
        VERSION 0.1.0
        DESCRIPTION "Labs and assignments completed in association with Graphics Programming / prog2002 at NTNU"
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
        $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wall>
        $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wextra>
        $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wpedantic>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wshadow>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wnon-virtual-dtor>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Woverloaded-virtual>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wmisleading-indentation>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wduplicated-cond>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wduplicated-branches>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wlogical-op>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wnull-dereference>
    $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wuseless-cast>
        $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wunreachable-code>
        $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-Wmissing-noreturn>
        $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wdocumentation>
        $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wweak-vtables>
        $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wexit-time-destructors>
        $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wglobal-constructors>)

# Dependencies
##############################################################################
# Find or download dependencies

find_package(OpenGL REQUIRED)

find_package(glfw3 3.3 CONFIG QUIET)
find_package(glm CONFIG QUIET)
find_package(Assimp CONFIG QUIET)
find_package(imgui CONFIG QUIET)
find_package(GLEW 2.1 MODULE QUIET)
find_package(Stb MODULE QUIET)

if (NOT TARGET assimp::assimp)
    if (Assimp_FOUND)
        add_library(assimp::assimp INTERFACE IMPORTED)
        target_link_libraries(assimp::assimp INTERFACE assimp)
    endif ()
endif ()

# FIXME: This does not build
if (NOT Assimp_FOUND)
    message(STATUS "Assimp not found locally, downloading...")
    FetchContent_Declare(
            Assimp
            GIT_REPOSITORY https://github.com/assimp/assimp
            GIT_TAG v5.0.1)

    # Disable a bunch of stuff we don't need
    # Build assimp dependencies with assimp
    set(ASSIMP_BUILD_ZLIB ON)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
    set(ASSIMP_BUILD_SAMPLES OFF)
    set(ASSIMP_BUILD_TESTS OFF)
    set(SYSTEM_IRRXML OFF)
    FetchContent_MakeAvailable(Assimp)
endif ()

if (NOT Stb_FOUND)
    message(STATUS "Stb not found locally, downloading...")
    FetchContent_Declare(
            Stb
            GIT_REPOSITORY https://github.com/nothings/stb
            GIT_TAG master)

    FetchContent_GetProperties(Stb)
    if (NOT Stb_POPULATED)
        FetchContent_Populate(Stb)

        add_library(Stb::Stb INTERFACE IMPORTED)
        target_include_directories(Stb::Stb INTERFACE "${Stb_SOURCE_DIR}")
    endif ()
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

# Things to do only if current project is top level project
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Setup compile_commands for clangd
    # TODO: This is a hack and I should find a better way to do this
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    execute_process(
            COMMAND
            ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json)

    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Enable testing
    if (BUILD_TESTING)
        add_subdirectory(tests)
    endif ()
endif ()

add_subdirectory(lib)
add_subdirectory(apps)
add_subdirectory(docs)
