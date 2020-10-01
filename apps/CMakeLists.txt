# Copy resource files to output directory if targets depend on them
# We could alternatively symlink these files instead
add_custom_target(copy-resources COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources
        )

# Labs
###############################################################################

add_executable(triangle triangle.cpp)
add_dependencies(triangle copy-resources)
target_compile_options(triangle PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(triangle PRIVATE lib)
target_precompile_headers(triangle REUSE_FROM lib)

add_executable(square square.cpp)
add_dependencies(square copy-resources)
target_compile_options(square PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(square PRIVATE lib)
target_precompile_headers(square REUSE_FROM lib)

add_executable(mvp mvp.cpp)
add_dependencies(mvp copy-resources)
target_compile_options(mvp PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(mvp PRIVATE lib)
target_precompile_headers(mvp REUSE_FROM lib)

add_executable(texture texture.cpp)
add_dependencies(texture copy-resources)
target_compile_options(texture PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(texture PRIVATE lib)
target_precompile_headers(texture REUSE_FROM lib)

# Assignments
###############################################################################

add_executable(pacman pacman.cpp Entities.cpp)
add_dependencies(pacman copy-resources)
target_compile_options(pacman PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(pacman PRIVATE lib)
target_precompile_headers(pacman REUSE_FROM lib)