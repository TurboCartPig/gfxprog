#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glove/Model.h>

void process_node(const aiScene *scene, const aiNode *node,
                  std::vector<Vertex3DNormTex> &vertices,
                  std::vector<uint32_t> &       indices) {
	for (size_t i = 0; i < node->mNumMeshes; ++i) {
		const auto *mesh = scene->mMeshes[node->mMeshes[i]];

		for (size_t j = 0; j < mesh->mNumVertices; ++j) {
			assert(mesh->HasPositions());
			assert(mesh->HasNormals());
			assert(mesh->HasTextureCoords(0));

			auto position =
			    glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y,
			              mesh->mVertices[j].z);
			auto normal = glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y,
			                        mesh->mNormals[j].z);
			auto uv     = glm::vec2(mesh->mTextureCoords[0][j].x,
                                mesh->mTextureCoords[0][j].y);

			vertices.push_back({position, normal, uv});
		}

		for (size_t j = 0; j < mesh->mNumFaces; ++j) {
			const auto &face = mesh->mFaces[j];
			for (size_t k = 0; k < face.mNumIndices; ++k) {
				indices.push_back(face.mIndices[k]);
			}
		}
	}

	for (size_t i = 0; i < node->mNumChildren; ++i) {
		process_node(scene, node->mChildren[i], vertices, indices);
	}
}

Model::Model(const std::string &model_path) {
	Assimp::Importer importer;

	const auto *scene =
	    importer.ReadFile(model_path, aiProcessPreset_TargetRealtime_Quality);

	assert(scene != nullptr);
	assert(scene->mRootNode != nullptr);
	assert(!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE));
	assert(scene->HasMeshes());

	std::vector<Vertex3DNormTex> vertices;
	std::vector<uint32_t>        indices;
	process_node(scene, scene->mRootNode, vertices, indices);

	m_vbo = std::make_unique<VertexBuffer<Vertex3DNormTex>>(vertices, indices);
}

void Model::draw() { m_vbo->draw(); }
