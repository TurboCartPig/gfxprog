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
				const auto offset = glm::vec3(i, 0.0f, j);
				auto index_offset = static_cast<uint32_t>(vertices.size());

				// Add 4 * 6 = 24 vertices
				// Add 36 indices

				// Add bottom face
				//				vertices.push_back(Vertex3DNormTex{
				//				    offset + glm::vec3(0.0f, 0.0f, 0.0f),
				//				    glm::vec3(0.0f, -1.0f, 0.0f),
				// glm::vec2(0.0f, 0.0f)});
				// vertices.push_back(Vertex3DNormTex{ 				    offset +
				// glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f),
				// glm::vec2(0.0f, 0.0f)}); vertices.push_back(Vertex3DNormTex{
				// offset + glm::vec3(1.0f, 0.0f, 0.0f),
				// glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
				// vertices.push_back(Vertex3DNormTex{ offset + glm::vec3(1.0f,
				// 0.0f, 1.0f), 				    glm::vec3(0.0f, -1.0f,
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
			const auto i = distribution(generator) * w;
			const auto j = distribution(generator) * h;
			if (level.get(static_cast<int>(i), static_cast<int>(j)) ==
			    EntityType::Tunnel) {
				const auto offset =
				    glm::vec3((float)i + 0.5f, 0.0f, (float)j + 0.5f);
				ghosts.emplace_back(Ghost(offset));
				break;
			}
		}
	}

	return ghosts;
}

std::unique_ptr<Pellets> genPellets(const Level &level) {
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

Pellets::Pellets(std::vector<glm::vec3> centroids)
    : m_centroids(std::move(centroids)) {
	using namespace std::string_literals;

	const auto shaders = {"resources/shaders/pellets.vert"s,
	                      "resources/shaders/model.frag"s};
	m_shader_program   = std::make_unique<ShaderProgram>(shaders);
	m_sphere           = std::make_unique<Model>("resources/models/sphere.obj");

	auto model_color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

	auto directional_light = DirectionalLight{
	    glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.25f, 1.0f, 0.5f), 2.0f};

	m_shader_program->use();
	m_shader_program->setUniform("u_model_color", model_color);
	m_shader_program->setUniform("u_directional_light", directional_light);
	m_shader_program->setUniform("u_diffuse_map", 0u);

	// Build uniform buffer with data from centroids
	std::vector<glm::mat4> transforms;
	transforms.reserve(m_centroids.size());
	for (const auto &centroid : m_centroids) {
		const auto transform = glm::scale(
		    glm::translate(glm::mat4(1.0f), centroid), glm::vec3(0.3f));
		transforms.push_back(transform);
	}

	m_sphere->setInstanceArray(transforms);
}

void Pellets::update(Pacman &pacman) {
	// Check for collision with pacman, and delete colliding pellets.
	for (const auto &centroid : m_centroids) {
		if (glm::length(centroid - pacman.getPosition()) <= 0.30f) {
			std::cout << "Collision detected!" << std::endl;
		}
	}

	// TODO: Update transforms if they changed
}

void Pellets::draw(const glm::mat4 &view, const glm::mat4 &projection) const {
	m_shader_program->use();
	m_shader_program->setUniform("u_view", view);
	m_shader_program->setUniform("u_projection", projection);

	m_sphere->draw();
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
	    level.get(static_cast<int>(std::round(translation.x - 0.5f)),
	              static_cast<int>(std::round(translation.z - 0.5f))) ==
	    EntityType::Wall;
	if (!collision) {
		m_transform.translation = translation;
	}
}

Ghost::Ghost(glm::vec3 position) {}
