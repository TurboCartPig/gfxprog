#pragma once

#include "Level3d.h"

#include <glove/lib.h>

void genLevelMesh(const Level &level, std::vector<Vertex3DNormTex> &vertices,
                  std::vector<uint32_t> &indices);

class Maze {
  public:
	Maze(const Level &level) {
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
	Pacman();
	void input(Input input);
	void update();

  private:
};

class Ghost {
  public:
	Ghost();
	void update();
	void draw() const;

  private:
};

class Pellet {
  public:
	Pellet();
	void draw() const;

  private:
};