add_library(lib lib.cpp)
target_compile_options(lib PRIVATE ${COMPILER_WARNINGS_AND_ERRORS})
target_include_directories(lib PUBLIC ../include)
