add_custom_target(copy-resources-to-tests COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)

find_package(Catch2 2.12 QUIET)

if (Catch2_FOUND)
    message(STATUS "Enabling tests")

    add_executable(tests test-lib.cpp)
    add_dependencies(tests copy-resources-to-tests)
    target_compile_options(tests PRIVATE ${COMPILER_WARNINGS_AND_ERRORS})
    target_link_libraries(tests PRIVATE Catch2::Catch2 lib)
    set_target_properties(
            tests
            PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            FOLDER "Tests")
endif ()