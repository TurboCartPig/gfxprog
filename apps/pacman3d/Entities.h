#pragma once

#include <glove/lib.h>

/**
 * @brief Generate the maze mesh from the level.
 *
 * @param level The level.
 * @param[out] vertices Unique vertices in the mesh.
 * @param[out] indices Indices representing the mesh.
 */
void genLevelMesh(const class Level &           level,
                  std::vector<Vertex3DNormTex> &vertices,
                  std::vector<uint32_t> &       indices);

/**
 * @brief Find pacman in the level.
 *
 * @param level Level.
 * @return glm::vec3 Position of pacman in the level.
 */
glm::vec3 findPacman(const class Level &level);

/**
 * @brief Generate ghosts based on their position in the level.
 *
 * @param level Level.
 * @return std::vector<class Ghost> Ghosts from the level.
 */
std::vector<class Ghost> genGhosts(const class Level &level);

/**
 * @brief Generate pellets based on the level.
 *
 * @param level Level.
 * @return std::unique_ptr<class Pellets> Pellets in the level.
 */
std::unique_ptr<class Pellets> genPellets(const class Level &level);

/**
 * @brief The level Maze.
 */
class Maze {
  public:
	Maze(const class Level &level) {
		std::vector<Vertex3DNormTex> vertices;
		std::vector<uint32_t>        indices;
		genLevelMesh(level, vertices, indices);
		m_vbo =
		    std::make_unique<VertexBuffer<Vertex3DNormTex>>(vertices, indices);
	}

	void draw() const { m_vbo->draw(); }

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
	void update(class Pacman &pacman);
	void draw(const glm::mat4 &view, const glm::mat4 &projection) const;

  private:
	std::unique_ptr<Model>         m_sphere;
	std::unique_ptr<ShaderProgram> m_shader_program;
	std::vector<glm::vec3>         m_centroids;
};

/**
 * @brief Pacman.
 * Also owns the camera used to render the game from first person perspective.
 */
class Pacman {
  public:
	Pacman(glm::vec3 position);
	void      input(Input input);
	void      update(float dt, const class Level &level);
	glm::vec3 getPosition() const { return m_transform.translation; }
	glm::mat4 view() const { return m_camera.view(m_transform); }
	glm::mat4 projection() const { return m_camera.projection(); }

  private:
	glm::vec3          m_forward;
	TransformComponent m_transform;
	CameraComponent    m_camera;
};

/**
 * @brief Ghost
 */
class Ghost {
  public:
	Ghost(glm::vec3 position);
	void update(float dt);
	void draw() const;

  private:
	glm::vec3          m_forward;
	TransformComponent m_transform;
};
