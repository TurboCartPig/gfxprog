#pragma once

#include <glove/lib.h>

void genLevelMesh(const class Level &           level,
                  std::vector<Vertex3DNormTex> &vertices,
                  std::vector<uint32_t> &       indices);

glm::vec3 findPacman(const class Level &level);

std::vector<class Ghost> genGhosts(const class Level &level);

std::vector<class Pellet> genPellets(const class Level &level);

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

class Pacman {
  public:
	Pacman(glm::vec3 position);
	void input(Input input);
	void      update(float dt);
	glm::mat4 viewProjection() const;

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

class Pellet {
  public:
	Pellet(glm::vec3 position);

  private:
	glm::vec3 m_position;
};