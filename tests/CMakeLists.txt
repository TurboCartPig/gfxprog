set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Catch2)

if (NOT Catch2_FOUND)
    message(STATUS "Enabling tests")

    add_executable(tests test-lib.cpp)
    target_compile_options(tests PRIVATE ${COMPILER_WARNINGS_AND_ERRORS})
    target_link_libraries(tests PRIVATE Catch2::Catch2 lib)
endif ()