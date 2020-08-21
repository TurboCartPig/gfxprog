find_package(Catch2)

message("-- Enabling tests")

# Testsuites are added as executable targets
add_executable(test-lib test-lib.cpp)
target_link_libraries(test-lib PRIVATE Catch2::Catch2 lib)
add_test(NAME test-lib-test COMMAND test-lib)
