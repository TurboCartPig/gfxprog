#pragma once

#include <glove/Texture.h>
#include <glove/VertexBuffer.h>
#include <glove/VertexFormats.h>
#include <memory>
#include <string>

/**
 * @brief A model representing one single mesh and accompanying texture maps.
 * Supports drawing and instanced drawing.
 * FIXME: Texture maps are not loaded automatically
 */
class Model {
  public:
	/**
	 * @brief Construct a new Model object from a model file.
	 *
	 * @param model_path Path to assimp compatible model file.
	 */
	explicit Model(const std::string &model_path);

	/**
	 * @brief Draw the model.
	 */
	void draw();

	/**
	 * @brief Associate instancing information with the model.
	 * Instanced drawing will be automatically performed from the time this
	 * method is called.
	 *
	 * @tparam InstanceFormat Format of the per-instance data.
	 * @param instances Per instance data.
	 */
	// template <typename InstanceFormat>
	// void setInstanceArray(const std::vector<InstanceFormat> &instances);

	template <typename InstanceFormat>
	void enableInstancing();

	template <typename InstanceFormat>
	void uploadInstanceData(const std::vector<InstanceFormat> &instance_data);

  private:
	std::unique_ptr<VertexBuffer<Vertex3DNormTex>>
	                         m_vbo;     ///< Internal VBO containing the mesh.
	std::unique_ptr<Texture> m_texture; ///< Internal Texture.
};
