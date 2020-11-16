#pragma once

#include <glove/lib.h>
#include <memory>

/**
 * @brief Generate the maze mesh from the level.
 *
 * @param level The level.
 */
auto genLevelMesh(const class Level &level)
-> std::pair<std::vector<Vertex3DNormTex>, std::vector<uint32_t>>;

/**
 * @brief Find pacman in the level.
 *
 * @param level Level.
 * @return glm::vec3 Position of pacman in the level.
 */
auto findPacman(const class Level &level) -> glm::vec3;

/**
 * @brief Generate ghosts based on their position in the level.
 *
 * @param level Level.
 * @return std::vector<class Ghost> Ghosts from the level.
 */
auto genGhosts(const class Level &level) -> std::vector<class Ghost>;

/**
 * @brief Generate pellets based on the level.
 *
 * @param level Level.
 * @return std::unique_ptr<class Pellets> Pellets in the level.
 */
auto genPellets(const class Level &level) -> std::unique_ptr<class Pellets>;
