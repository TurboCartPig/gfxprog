#pragma once

#include <glove/Texture.h>
#include <glove/VertexBuffer.h>
#include <glove/VertexFormats.h>
#include <memory>
#include <string>

class Model {
  public:
	explicit Model(const std::string &model_path);

	void draw();

  private:
	std::unique_ptr<VertexBuffer<Vertex3DNormTex>> m_vbo;
	std::unique_ptr<Texture>                       m_texture;
};
