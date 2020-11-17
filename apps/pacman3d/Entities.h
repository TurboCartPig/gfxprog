#pragma once

#include <glove/lib.h>

/**
 * @brief The level Maze.
 */
class Maze {
  public:
	Maze(const class Level &level);

	/**
	 * @brief Draw the entire maze.
	 */
	void draw() const { m_vbo->draw(); }

	/**
	 * @brief Get the transformation matrix for rendering.
	 * @return The transformation matrix.
	 */
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

	/**
	 * @brief Update the internal state of all the pellets.
	 * @param pacman Pacman entity.
	 * @return Has pacman picked up all the pellets?
	 */
	[[nodiscard]] auto update(const class Pacman &pacman) -> bool;

	/**
	 * @brief Draw all the pellets.
	 */
	void draw() const;

  private:
	/**
	 * @brief Build and upload the instance data for all the pellets.
	 */
	void upload() const;

  private:
	std::unique_ptr<Model> m_sphere;
	std::vector<glm::vec3>
	    m_centroids; ///< Center positions for all the pellets.
};

/**
 * @brief Pacman.
 * Also owns the camera used to render the game from first person perspective.
 */
class Pacman {
  public:
	Pacman(glm::vec3 position);

	/**
	 * @brief Pass input to pacman.
	 * @param input Input from the window.
	 */
	void input(Input input);

	/**
	 * @brief Update the internal state of pacman.
	 * @param dt Delta time.
	 * @param level The level grid.
	 */
	void update(float dt, const class Level &level);

	/**
	 * @brief Draw pacman to the minimap.
	 */
	void draw() const;

	/**
	 * @brief Update the aspect ratio of pacman's camera.
	 * @param aspect Aspect ratio.
	 */
	void updateAspectRatio(float aspect);

	/**
	 * @brief Get pacman's position.
	 * @return Pacman's position.
	 */
	[[nodiscard]] auto getPosition() const { return m_transform.translation; }

	/**
	 * @brief Get the view matrix from pacman's camera.
	 * @return The view matrix.
	 */
	[[nodiscard]] auto view() const { return m_camera.view(m_transform); }

	/**
	 * @brief Get the projection matrix from pacman's camera.
	 * @return The projection matrix.
	 */
	[[nodiscard]] auto projection() const { return m_camera.projection(); }

	/**
	 * @brief Get pacman's transform matrix.
	 * @return The transform matrix.
	 */
	[[nodiscard]] auto getTransform() const { return m_transform.matrix(); }

  private:
	float                  m_yaw;     ///< Yaw for delta rotation from input.
	glm::vec3              m_forward; ///< Forward direction based on input.
	TransformComponent     m_transform;
	CameraComponent        m_camera;
	std::unique_ptr<Model> m_model; ///< Model for the minimap.
};

/**
 * @brief Ghost.
 */
class Ghost {
  public:
	Ghost(glm::vec3 position, std::shared_ptr<Model> model);

	/**
	 * @brief Update the internal state of an individual Ghost.
	 * @param dt Delta time.
	 * @param pacman Pacman entity.
	 * @param level Level grid.
	 * @return Did this ghost collide with pacman?
	 */
	[[nodiscard]] auto update(float dt, const Pacman &pacman,
	                          const class Level &level) -> bool;

	/**
	 * @brief Draw the ghost.
	 */
	void draw() const;

	/**
	 * @brief Get the transformation matrix for passing to shaders.
	 * @return Entities transformation matrix.
	 */
	[[nodiscard]] auto getTransform() const { return m_transform.matrix(); }

  private:
	glm::vec3              m_forward; ///< Forward direction based on input.
	TransformComponent     m_transform;
	std::shared_ptr<Model> m_model;
};
