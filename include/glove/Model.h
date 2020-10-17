#pragma once

#include <filesystem>
#include <glove/Texture.h>
#include <glove/VertexBuffer.h>
#include <glove/VertexFormats.h>
#include <memory>

class Model {
  public:
	explicit Model(const std::filesystem::path &model_path);

	void draw();

  private:
	std::unique_ptr<VertexBuffer<Vertex3DNormTex>> m_vbo;
	std::unique_ptr<Texture>                       m_texture;
};
