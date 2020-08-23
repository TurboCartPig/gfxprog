find_package(Catch2)

message("-- Enabling tests")

add_executable(tests test-lib.cpp)
target_compile_options(tests PRIVATE ${COMPILER_WARNINGS_AND_ERRORS})
target_link_libraries(tests PRIVATE Catch2::Catch2 lib)