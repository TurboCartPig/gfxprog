#include "Entities.h"

#include "Level.h"
#include "generation.h"

#include <algorithm>
#include <random>
#include <utility>

Maze::Maze(const Level &level) {
	auto [vertices, indices] = genLevelMesh(level);
	m_vbo = std::make_unique<VertexBuffer<Vertex3DNormTex>>(vertices, indices);
}

Pellets::Pellets(std::vector<glm::vec3> centroids)
    : m_centroids(std::move(centroids)) {
	m_sphere = std::make_unique<Model>("resources/models/sphere.obj");
	m_sphere->enableInstancing<glm::mat4>();
	upload();
}

auto Pellets::update(const Pacman &pacman) -> bool {
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

	// Return true if pacman has eaten all the pellets
	return m_centroids.empty();
}

void Pellets::draw() const { m_sphere->draw(); }

void Pellets::upload() const {
	// Build instance data from centroids
	std::vector<glm::mat4> transforms;
	transforms.reserve(m_centroids.size());

	for (const auto &centroid : m_centroids) {
		const auto transform = glm::scale(
		    glm::translate(glm::mat4(1.0f), centroid), glm::vec3(0.2f));
		transforms.push_back(transform);
	}

	// Upload the instance data
	m_sphere->uploadInstanceData(transforms);
}

Pacman::Pacman(glm::vec3 position) : m_yaw(0.0f) {
	m_forward   = glm::vec3(0.0f, 0.0f, 0.0f);
	m_transform = {position, glm::vec3(0.0f), glm::vec3(0.25f)};
	// FIXME: Aspect ratio needs to be updated when the window is resized
	m_camera = CameraComponent(16.0f / 9.0f, 96.0f);
	m_model = std::make_unique<Model>("resources/models/sphere.obj");
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
		} else if (input.code == InputCode::Left) {
			m_yaw += 1.25f;
		} else if (input.code == InputCode::Right) {
			m_yaw -= 1.25f;
		}
	} else if (input.state == InputState::Released) {
		if (input.code == InputCode::W || input.code == InputCode::S) {
			m_forward.z = 0.0f;
		} else if (input.code == InputCode::A || input.code == InputCode::D) {
			m_forward.x = 0.0f;
		} else if (input.code == InputCode::Left || input.code == InputCode::Right) {
			m_yaw = 0.0f;
		}
	}
}

void Pacman::update(float dt, const Level &level) {
	m_transform.rotation.y += m_yaw * dt;

	const auto translation =
	    m_transform.translation +
	    glm::vec3(glm::eulerAngleY(m_transform.rotation.y) *
	              glm::vec4(m_forward, 1.0f)) *
	        dt;

	const auto collision =
	    level.get(static_cast<int>(std::round(translation.x - 0.5f)),
	              static_cast<int>(std::round(translation.z - 0.5f))) ==
	    EntityType::Wall;
	if (!collision) {
		m_transform.translation = translation;
	}
}

void Pacman::draw() const {
	m_model->draw();
}

void Pacman::updateAspectRatio(float aspect) {
	m_camera.aspect = aspect;
}

Ghost::Ghost(glm::vec3 position, std::shared_ptr<Model> model)
    : m_model(std::move(model)) {
	m_forward   = glm::vec3(1.0f, 0.0f, 0.0f);
	m_transform = {position, glm::vec3(0.0f), glm::vec3(0.25f)};
}

auto Ghost::update(float dt, const Pacman &pacman, const Level &level) -> bool {
	// Find new pos based on pos, forward and dt
	const auto translation = m_transform.translation + m_forward * dt;

	// Check collision against grid
	const auto collision =
	    level.get(static_cast<int>(std::round(translation.x - 0.5f)),
	              static_cast<int>(std::round(translation.z - 0.5f))) ==
	    EntityType::Wall;

	// FIXME: They currently get stuck because of the order of branches

	// Change direction if they will collide
	if (!collision) {
		m_transform.translation = translation;
	} else if (m_forward.x == -1.0f) {
		// Float comparison is fine because the are checked
		// against what they have been set to and whole numbers
		// will always be represented accurately.
		m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	} else if (m_forward.z == -1.0f) {
		m_forward = glm::vec3(1.0f, 0.0f, 0.0f);
	} else if (m_forward.x == 1.0f) {
		m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
	} else {
		m_forward = glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	const auto pacman_collision =
	    glm::length(pacman.getPosition() - m_transform.translation) <= 0.75f;

	// If collide with pacman, return true
	return pacman_collision;
}

void Ghost::draw() const { m_model->draw(); }
