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

	template <typename InstanceFormat>
	void setInstanceArray(const std::vector<InstanceFormat> &instances);

  private:
	std::unique_ptr<VertexBuffer<Vertex3DNormTex>> m_vbo;
	std::unique_ptr<Texture>                       m_texture;
};
