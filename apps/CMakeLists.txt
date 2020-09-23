add_executable(triangle triangle.cpp)
target_compile_options(triangle PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(triangle PRIVATE lib)
target_precompile_headers(triangle REUSE_FROM lib)

add_executable(square square.cpp)
target_compile_options(square PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(square PRIVATE lib)
target_precompile_headers(square REUSE_FROM lib)

add_executable(mvp mvp.cpp)
target_compile_options(mvp PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(mvp PRIVATE lib)
target_precompile_headers(mvp REUSE_FROM lib)

add_executable(pacman pacman.cpp)
target_compile_options(pacman PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(pacman PRIVATE lib)
target_precompile_headers(pacman REUSE_FROM lib)