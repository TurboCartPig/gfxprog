#pragma once

#include <glove/lib.h>

/**
 * @brief The level Maze.
 */
class Maze {
  public:
	Maze(const class Level &level);
	void               draw() const { m_vbo->draw(); }
	[[nodiscard]] auto getTransform() const { return glm::mat4(1.0f); }

  private:
	std::unique_ptr<VertexBuffer<Vertex3DNormTex>> m_vbo;
};

/**
 * @brief The pellets in the level.
 * Implements drawing all the pellets as spheres using instanced rendering, in
 * addition to detecting collisions between pellets and pacman.
 */
class Pellets {
  public:
	Pellets(std::vector<glm::vec3> centroids);
	[[nodiscard]] auto update(const class Pacman &pacman) -> bool;
	void               draw() const;

  private:
	/**
	 * @brief Build and upload the instance data for all the pellets.
	 */
	void upload() const;

  private:
	std::unique_ptr<Model> m_sphere;
	std::vector<glm::vec3> m_centroids;
};

/**
 * @brief Pacman.
 * Also owns the camera used to render the game from first person perspective.
 */
class Pacman {
  public:
	Pacman(glm::vec3 position);
	void               input(Input input);
	void               update(float dt, const class Level &level);
	[[nodiscard]] auto getPosition() const { return m_transform.translation; }
	[[nodiscard]] auto view() const { return m_camera.view(m_transform); }
	[[nodiscard]] auto projection() const { return m_camera.projection(); }

  private:
	glm::vec3          m_forward;
	TransformComponent m_transform;
	CameraComponent    m_camera;
};

/**
 * @brief Ghost.
 */
class Ghost {
  public:
	Ghost(glm::vec3 position, std::shared_ptr<Model> model);
	[[nodiscard]] auto update(float dt, const Pacman &pacman,
	                          const class Level &level) -> bool;
	void               draw() const;
	[[nodiscard]] auto getTransform() const { return m_transform.matrix(); }

  private:
	glm::vec3              m_forward;
	TransformComponent     m_transform;
	std::shared_ptr<Model> m_model;
};
