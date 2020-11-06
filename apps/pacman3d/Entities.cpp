#include "Entities.h"

#include "Level.h"

#include <random>

void genLevelMesh(const Level &level, std::vector<Vertex3DNormTex> &vertices,
                  std::vector<uint32_t> &indices) {
	const auto [w, h] = level.getSize();

	// Add floor
	vertices.push_back(Vertex3DNormTex{glm::vec3(0.0f, 0.0f, 0.0f),
	                                   glm::vec3(0.0f, -1.0f, 0.0f),
	                                   glm::vec2(0.0f, 0.0f)});
	vertices.push_back(Vertex3DNormTex{glm::vec3(0.0f, 0.0f, (float)h),
	                                   glm::vec3(0.0f, -1.0f, 0.0f),
	                                   glm::vec2(0.0f, 0.0f)});
	vertices.push_back(Vertex3DNormTex{glm::vec3((float)w, 0.0f, 0.0f),
	                                   glm::vec3(0.0f, -1.0f, 0.0f),
	                                   glm::vec2(0.0f, 0.0f)});
	vertices.push_back(Vertex3DNormTex{glm::vec3((float)w, 0.0f, (float)h),
	                                   glm::vec3(0.0f, -1.0f, 0.0f),
	                                   glm::vec2(0.0f, 0.0f)});

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// Add individual cubes, but only the faces to face inward, upward or
	// downward, not faces facing other faces within the maze.
	for (int j = 0; j < h; ++j) {
		for (int i = 0; i < w; ++i) {
			const auto entity = level.get(i, j);
			if (entity == EntityType::Wall) {
				const auto offset       = glm::vec3(i, 0.0f, j);
				auto       index_offset = vertices.size();

				// Add 4 * 6 = 24 vertices
				// Add 36 indices

				// Add bottom face
				//				vertices.push_back(Vertex3DNormTex{
				//				    offset + glm::vec3(0.0f, 0.0f, 0.0f),
				//				    glm::vec3(0.0f, -1.0f, 0.0f),
				//glm::vec2(0.0f, 0.0f)});
				// vertices.push_back(Vertex3DNormTex{ 				    offset +
				// glm::vec3(0.0f, 0.0f, 1.0f), 				    glm::vec3(0.0f, -1.0f,
				// 0.0f), glm::vec2(0.0f, 0.0f)});
				// vertices.push_back(Vertex3DNormTex{ 				    offset + glm::vec3(1.0f,
				//0.0f, 0.0f), 				    glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f,
				// 0.0f)}); 				vertices.push_back(Vertex3DNormTex{
				// offset +
				// glm::vec3(1.0f, 0.0f, 1.0f), 				    glm::vec3(0.0f, -1.0f,
				// 0.0f), glm::vec2(0.0f, 0.0f)});
				//
				//				indices.push_back(index_offset + 0);
				//				indices.push_back(index_offset + 1);
				//				indices.push_back(index_offset + 2);
				//				indices.push_back(index_offset + 1);
				//				indices.push_back(index_offset + 2);
				//				indices.push_back(index_offset + 3);
				//				index_offset += 4;

				// Add top face
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(0.0f, 1.0f, 0.0f),
				    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(0.0f, 1.0f, 1.0f),
				    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(1.0f, 1.0f, 0.0f),
				    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(1.0f, 1.0f, 1.0f),
				    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});

				indices.push_back(index_offset + 0);
				indices.push_back(index_offset + 1);
				indices.push_back(index_offset + 2);
				indices.push_back(index_offset + 1);
				indices.push_back(index_offset + 2);
				indices.push_back(index_offset + 3);
				index_offset += 4;

				if (j - 1 >= 0 && level.get(i, j - 1) != EntityType::Wall) {
					// Add south face
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 0.0f, 0.0f),
					    glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 0.0f, 0.0f),
					    glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 1.0f, 0.0f),
					    glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 1.0f, 0.0f),
					    glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});

					indices.push_back(index_offset + 0);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 3);
					index_offset += 4;
				}

				// Add north face
				if (j + 1 < h && level.get(i, j + 1) != EntityType::Wall) {
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 0.0f, 1.0f),
					    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 0.0f, 1.0f),
					    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 1.0f, 1.0f),
					    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 1.0f, 1.0f),
					    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});

					indices.push_back(index_offset + 0);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
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
					    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 1.0f, 0.0f),
					    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 1.0f, 1.0f),
					    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});

					indices.push_back(index_offset + 0);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
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
					    glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 1.0f, 0.0f),
					    glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 1.0f, 1.0f),
					    glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});

					indices.push_back(index_offset + 0);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 3);
					index_offset += 4;
				}
			}
		}
	}
}

glm::vec3 findPacman(const Level &level) {
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

std::vector<Ghost> genGhosts(const Level &level) {
	const auto [w, h] = level.getSize();
	std::vector<Ghost> ghosts;
	ghosts.reserve(4);

	std::random_device                    rd;
	std::default_random_engine            generator(rd());
	std::uniform_real_distribution<float> distribution(0.0, 1.0);

	for (int k = 0; k < 4; k++) {
		while (true) {
			int i = distribution(generator) * w;
			int j = distribution(generator) * h;
			if (level.get(i, j) == EntityType::Tunnel) {
				const auto offset =
				    glm::vec3((float)i + 0.5f, 0.0f, (float)j + 0.5f);
				ghosts.emplace_back(Ghost(offset));
				break;
			}
		}
	}

	return ghosts;
}

std::vector<Pellet> genPellets(const Level &level) {
	const auto [w, h] = level.getSize();
	std::vector<Pellet> pellets;

	for (int j = 0; j < h; ++j) {
		for (int i = 0; i < w; ++i) {
			const auto entity = level.get(i, j);
			const auto offset =
			    glm::vec3((float)i + 0.5f, 0.0f, (float)j + 0.5f);
			if (entity == EntityType::Tunnel) {
				pellets.emplace_back(offset);
			}
		}
	}

	return pellets;
}

Pacman::Pacman(glm::vec3 position) {
	m_forward   = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat q = glm::quatLookAt(glm::vec3(0.0f, 0.0f, -1.0f),
	                              glm::vec3(0.0f, 1.0f, 0.0f));
	m_transform = {position, q, glm::vec3(1.0f)};
	m_camera    = CameraComponent(16.0f / 9.0f, 96.0f);
}

void Pacman::input(Input input) {
	if (input.state == InputState::Pressed) {
		if (input.code == InputCode::W) {
			m_forward.z = 1.0f;
		} else if (input.code == InputCode::S) {
			m_forward.z = -1.0f;
		} else if (input.code == InputCode::A) {
			m_forward.x = 1.0f;
		} else if (input.code == InputCode::D) {
			m_forward.x = -1.0f;
		}
	} else if (input.state == InputState::Released) {
		if (input.code == InputCode::W || input.code == InputCode::S)
			m_forward.z = 0.0f;
		else if (input.code == InputCode::A || input.code == InputCode::D)
			m_forward.x = 0.0f;
	}
}

void Pacman::update(float dt, const Level &level) {
	auto translation = m_transform.translation + m_forward * dt;

	auto collision =
	    level.get(std::round(translation.x - 0.5f),
	              std::round(translation.z - 0.5f)) == EntityType::Wall;
	if (!collision) {
		m_transform.translation = translation;
	}
}

glm::mat4 Pacman::viewProjection() const {
	return m_camera.asViewProjection(m_transform);
}

Ghost::Ghost(glm::vec3 position) {}

Pellet::Pellet(glm::vec3 postition) { m_position = postition; }