#include "Entities.h"

#include "Level.h"

#include <algorithm>
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
	                                   glm::vec2(0.0f, 0.0f)});
	vertices.push_back(Vertex3DNormTex{glm::vec3((float)w, 0.0f, 0.0f),
	                                   glm::vec3(0.0f, 1.0f, 0.0f),
	                                   glm::vec2(0.0f, 0.0f)});
	vertices.push_back(Vertex3DNormTex{glm::vec3((float)w, 0.0f, (float)h),
	                                   glm::vec3(0.0f, 1.0f, 0.0f),
	                                   glm::vec2(0.0f, 0.0f)});

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
					    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 1.0f, 1.0f),
					    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 1.0f, 1.0f),
					    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});

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
					    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 1.0f, 0.0f),
					    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 1.0f, 1.0f),
					    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});

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

Maze::Maze(const Level &level) {
	auto [vertices, indices] = genLevelMesh(level);
	m_vbo = std::make_unique<VertexBuffer<Vertex3DNormTex>>(vertices, indices);
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

	upload();
}

void Pellets::update(Pacman &pacman) {
	auto before = m_centroids.size();

	// Check for collision with pacman, and delete colliding pellets.
	// TODO: Can we instead only check and remove the closets pellet, only one
	// can be picked up at a time anyway?
	auto remove = std::remove_if(
	    begin(m_centroids), end(m_centroids), [&](const auto &c) {
		    return glm::length(c - pacman.getPosition()) <= 0.4f;
	    });
	m_centroids.erase(remove, end(m_centroids));

	auto after = m_centroids.size();

	// Update transforms if they changed
	if (before != after)
		upload();
}

void Pellets::draw(const glm::mat4 &view, const glm::mat4 &projection) const {
	m_shader_program->use();
	m_shader_program->setUniform("u_view", view);
	m_shader_program->setUniform("u_projection", projection);

	m_sphere->draw();
}

void Pellets::upload() const {
	// Build uniform buffer with data from centroids
	std::vector<glm::mat4> transforms;
	transforms.reserve(m_centroids.size());
	for (const auto &centroid : m_centroids) {
		const auto transform = glm::scale(
		    glm::translate(glm::mat4(1.0f), centroid), glm::vec3(0.2f));
		transforms.push_back(transform);
	}

	// Upload the instance data
	m_sphere->enableInstancing<glm::mat4>();
	m_sphere->uploadInstanceData(transforms);
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
		if (input.code == InputCode::W || input.code == InputCode::S) {
			m_forward.z = 0.0f;
		} else if (input.code == InputCode::A || input.code == InputCode::D) {
			m_forward.x = 0.0f;
		}
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
