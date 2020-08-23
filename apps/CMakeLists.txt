add_executable(hello hello.cpp)
target_compile_options(hello PRIVATE ${COMPILER_WARNINGS_AND_ERRORS})
target_precompile_headers(hello PUBLIC <iostream>)

add_executable(add add.cpp)
target_compile_options(add PRIVATE ${COMPILER_WARNINGS_AND_ERRORS})
target_link_libraries(add lib)
target_precompile_headers(add REUSE_FROM hello)
