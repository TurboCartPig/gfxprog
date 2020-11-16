#include "generation.h"

#include "Level.h"
#include "Entities.h"

#include <random>

auto genLevelMesh(const Level &level)
-> std::pair<std::vector<Vertex3DNormTex>, std::vector<uint32_t>> {
    std::vector<uint32_t>        indices;
    std::vector<Vertex3DNormTex> vertices;

    const auto [w, h] = level.getSize();

    // Add floor
    vertices.push_back(Vertex3DNormTex{glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f),
                                       glm::vec2(0.0f, 0.0f)});
    vertices.push_back(Vertex3DNormTex{glm::vec3(0.0f, 0.0f, (float)h),
                                       glm::vec3(0.0f, 1.0f, 0.0f),
                                       glm::vec2(0.0f, 1.0f)});
    vertices.push_back(Vertex3DNormTex{glm::vec3((float)w, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f),
                                       glm::vec2(1.0f, 0.0f)});
    vertices.push_back(Vertex3DNormTex{glm::vec3((float)w, 0.0f, (float)h),
                                       glm::vec3(0.0f, 1.0f, 0.0f),
                                       glm::vec2(1.0f, 1.0f)});

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(1);

    // Add individual cubes, but only the faces to face inward, upward or
    // downward, not faces facing other faces within the maze.
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            const auto entity = level.get(i, j);
            if (entity == EntityType::Wall) {
                const auto offset = glm::vec3(i, 0.0f, j);
                auto index_offset = static_cast<uint32_t>(vertices.size());

                // Add top face
                vertices.push_back(Vertex3DNormTex{
                    offset + glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
                vertices.push_back(Vertex3DNormTex{
                    offset + glm::vec3(0.0f, 1.0f, 1.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
                vertices.push_back(Vertex3DNormTex{
                    offset + glm::vec3(1.0f, 1.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
                vertices.push_back(Vertex3DNormTex{
                    offset + glm::vec3(1.0f, 1.0f, 1.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)});

                indices.push_back(index_offset + 0);
                indices.push_back(index_offset + 1);
                indices.push_back(index_offset + 2);
                indices.push_back(index_offset + 3);
                indices.push_back(index_offset + 2);
                indices.push_back(index_offset + 1);
                index_offset += 4;

                if (j - 1 >= 0 && level.get(i, j - 1) != EntityType::Wall) {
                    // Add south face
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(1.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(1.0f, 1.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(0.0f, 1.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)});

                    indices.push_back(index_offset + 0);
                    indices.push_back(index_offset + 1);
                    indices.push_back(index_offset + 2);
                    indices.push_back(index_offset + 3);
                    indices.push_back(index_offset + 2);
                    indices.push_back(index_offset + 1);
                    index_offset += 4;
                }

                // Add north face
                if (j + 1 < h && level.get(i, j + 1) != EntityType::Wall) {
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(1.0f, 0.0f, 1.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(0.0f, 0.0f, 1.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(1.0f, 1.0f, 1.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(0.0f, 1.0f, 1.0f),
                        glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)});

                    indices.push_back(index_offset + 2);
                    indices.push_back(index_offset + 1);
                    indices.push_back(index_offset + 0);
                    indices.push_back(index_offset + 1);
                    indices.push_back(index_offset + 2);
                    indices.push_back(index_offset + 3);
                    index_offset += 4;
                }

                // Add east face
                if (i + 1 < w && level.get(i + 1, j) != EntityType::Wall) {
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(1.0f, 0.0f, 0.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(1.0f, 0.0f, 1.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(1.0f, 1.0f, 0.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(1.0f, 1.0f, 1.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)});

                    indices.push_back(index_offset + 2);
                    indices.push_back(index_offset + 1);
                    indices.push_back(index_offset + 0);
                    indices.push_back(index_offset + 1);
                    indices.push_back(index_offset + 2);
                    indices.push_back(index_offset + 3);
                    index_offset += 4;
                }

                // Add west face
                if (i - 1 >= 0 && level.get(i - 1, j) != EntityType::Wall) {
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(0.0f, 0.0f, 1.0f),
                        glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(0.0f, 1.0f, 0.0f),
                        glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)});
                    vertices.push_back(Vertex3DNormTex{
                        offset + glm::vec3(0.0f, 1.0f, 1.0f),
                        glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)});

                    indices.push_back(index_offset + 0);
                    indices.push_back(index_offset + 1);
                    indices.push_back(index_offset + 2);
                    indices.push_back(index_offset + 3);
                    indices.push_back(index_offset + 2);
                    indices.push_back(index_offset + 1);
                }
            }
        }
    }

    return std::make_pair(vertices, indices);
}

auto findPacman(const Level &level) -> glm::vec3 {
    const auto [w, h] = level.getSize();

    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            const auto entity = level.get(i, j);
            if (entity == EntityType::Pacman) {
                return glm::vec3((float)i + 0.5f, 0.5f, (float)j + 0.5f);
            }
        }
    }

    throw std::runtime_error("Pacman not found in level!");
}

auto genGhosts(const Level &level) -> std::vector<Ghost> {
    const auto [w, h] = level.getSize();
    std::vector<Ghost> ghosts;
    ghosts.reserve(4);

    // Load model
    auto model = std::make_shared<Model>("resources/models/sphere.obj");

    // Init random number generator
    std::random_device                    rd;
    std::default_random_engine            generator(rd());
    std::uniform_real_distribution<float> distribution(0.0, 1.0);

    for (int k = 0; k < 4; k++) {
        while (true) {
            const auto i = std::floor(distribution(generator) * w);
            const auto j = std::floor(distribution(generator) * h);
            if (level.get(static_cast<int>(i), static_cast<int>(j)) ==
                EntityType::Tunnel) {
                const auto offset = glm::vec3(i + 0.5f, 0.5f, j + 0.5f);
                ghosts.emplace_back(Ghost(offset, model));
                break;
            }
        }
    }

    return ghosts;
}

auto genPellets(const Level &level) -> std::unique_ptr<Pellets> {
    const auto [w, h] = level.getSize();
    std::vector<glm::vec3> pellets;

    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            const auto entity = level.get(i, j);
            const auto offset =
                glm::vec3((float)i + 0.5f, 0.25f, (float)j + 0.5f);
            if (entity == EntityType::Tunnel) {
                pellets.emplace_back(offset);
            }
        }
    }

    return std::make_unique<Pellets>(pellets);
}
