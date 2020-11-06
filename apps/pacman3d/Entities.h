#pragma once

#include <glove/lib.h>

void genLevelMesh(const class Level &           level,
                  std::vector<Vertex3DNormTex> &vertices,
                  std::vector<uint32_t> &       indices);

glm::vec3 findPacman(const class Level &level);

std::vector<class Ghost> genGhosts(const class Level &level);

std::unique_ptr<class Pellets> genPellets(const class Level &level);

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

class Pellets {
  public:
	Pellets(std::vector<glm::vec3> centroids);
	void update();
	void draw(const glm::mat4 &view, const glm::mat4 &projection) const;

  private:
	std::unique_ptr<Model>         m_sphere;
	std::unique_ptr<ShaderProgram> m_shader_program;
	std::vector<glm::vec3>         m_centroids;
};

class Pacman {
  public:
	Pacman(glm::vec3 position);
	void      input(Input input);
	void      update(float dt, const class Level &level);
	glm::mat4 view() { return m_camera.view(m_transform); }
	glm::mat4 projection() { return m_camera.projection(); }

  private:
	glm::vec3          m_forward;
	TransformComponent m_transform;
	CameraComponent    m_camera;
};

class Ghost {
  public:
	Ghost(glm::vec3 position);
	void update(float dt);
	void draw() const;

  private:
	glm::vec3          m_forward;
	TransformComponent m_transform;
};
