# Copy resource files to output directory if targets depend on them
# We could alternatively symlink these files instead
add_custom_target(copy-resources COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${CMAKE_CURRENT_BINARY_DIR}/resources)

# Labs
###############################################################################

add_executable(triangle triangle.cpp)
add_dependencies(triangle copy-resources)
target_compile_options(triangle PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(triangle PRIVATE lib)

add_executable(square square.cpp)
add_dependencies(square copy-resources)
target_compile_options(square PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(square PRIVATE lib)

add_executable(mvp mvp.cpp)
add_dependencies(mvp copy-resources)
target_compile_options(mvp PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(mvp PRIVATE lib)

add_executable(texture texture.cpp)
add_dependencies(texture copy-resources)
target_compile_options(texture PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(texture PRIVATE lib)

add_executable(model model.cpp)
add_dependencies(model copy-resources)
target_compile_options(model PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(model PRIVATE lib)

# Assignments
###############################################################################

file(GLOB PACMAN_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pacman/*.cpp")

add_executable(pacman "${PACMAN_SOURCE_FILES}")
add_dependencies(pacman copy-resources)
target_include_directories(pacman PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pacman")
target_compile_options(pacman PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(pacman PRIVATE lib)

file(GLOB PACMAN3D_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pacman3d/*.cpp")

add_executable(pacman3d "${PACMAN3D_SOURCE_FILES}")
add_dependencies(pacman3d copy-resources)
target_include_directories(pacman3d PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pacman3d")
target_compile_options(pacman3d PRIVATE ${COMPILER_WARNINGS_AND_ERROR})
target_link_libraries(pacman3d PRIVATE lib)

set_target_properties(
        triangle square mvp texture model pacman pacman3d
        PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        PRECOMPILE_HEADERS_REUSE_FROM lib
        MSVC_RUNTIME_LIBRARY "MultiThreadedDebug"
        FOLDER "Apps")