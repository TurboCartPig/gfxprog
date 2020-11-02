#include "Entities3d.h"

void genLevelMesh(const Level &level, std::vector<Vertex3DNormTex> &vertices,
                  std::vector<uint32_t> &indices) {
	auto [w, h] = level.getSize();

	// Add floor
	vertices.push_back(Vertex3DNormTex{glm::vec3(0.0f, 0.0f, 0.0f),
	                                   glm::vec3(0.0f, -1.0f, 0.0f),
	                                   glm::vec2(0.0f, 0.0f)});
	vertices.push_back(Vertex3DNormTex{glm::vec3(0.0f, 0.0f, (float)h),
	                                   glm::vec3(0.0f, -1.0f, 0.0f),
	                                   glm::vec2(0.0f, 0.0f)});
	vertices.push_back(Vertex3DNormTex{glm::vec3((float)w, 0.0f, 0.0f),
	                                   glm::vec3(0.0f, -1.0f, 0.0f),
	                                   glm::vec2(0.0f, 0.0f)});
	vertices.push_back(Vertex3DNormTex{glm::vec3((float)w, 0.0f, (float)h),
	                                   glm::vec3(0.0f, -1.0f, 0.0f),
	                                   glm::vec2(0.0f, 0.0f)});

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// Add individual cubes, but only the faces to face inward, upward or
	// downward, not faces facing other faces within the maze.
	for (int j = 0; j < h; ++j) {
		for (int i = 0; i < w; ++i) {
			const auto entity = level.get(i, j);
			if (entity == EntityType::Wall) {
				const auto offset       = glm::vec3(i, 0.0f, j);
				auto       index_offset = vertices.size();

				// Add 4 * 6 = 24 vertices
				// Add 36 indices

				// Add bottom face
//				vertices.push_back(Vertex3DNormTex{
//				    offset + glm::vec3(0.0f, 0.0f, 0.0f),
//				    glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
//				vertices.push_back(Vertex3DNormTex{
//				    offset + glm::vec3(0.0f, 0.0f, 1.0f),
//				    glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
//				vertices.push_back(Vertex3DNormTex{
//				    offset + glm::vec3(1.0f, 0.0f, 0.0f),
//				    glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
//				vertices.push_back(Vertex3DNormTex{
//				    offset + glm::vec3(1.0f, 0.0f, 1.0f),
//				    glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
//
//				indices.push_back(index_offset + 0);
//				indices.push_back(index_offset + 1);
//				indices.push_back(index_offset + 2);
//				indices.push_back(index_offset + 1);
//				indices.push_back(index_offset + 2);
//				indices.push_back(index_offset + 3);
//				index_offset += 4;

				// Add top face
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(0.0f, 1.0f, 0.0f),
				    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(0.0f, 1.0f, 1.0f),
				    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(1.0f, 1.0f, 0.0f),
				    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(1.0f, 1.0f, 1.0f),
				    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});

				indices.push_back(index_offset + 0);
				indices.push_back(index_offset + 1);
				indices.push_back(index_offset + 2);
				indices.push_back(index_offset + 1);
				indices.push_back(index_offset + 2);
				indices.push_back(index_offset + 3);
				index_offset += 4;

				if (j - 1 >= 0 && level.get(i, j - 1) != EntityType::Wall) {
					// Add south face
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 0.0f, 0.0f),
					    glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 0.0f, 0.0f),
					    glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 1.0f, 0.0f),
					    glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 1.0f, 0.0f),
					    glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)});

					indices.push_back(index_offset + 0);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 3);
					index_offset += 4;
				}

				// Add north face
				if (j + 1 < h && level.get(i, j + 1) != EntityType::Wall) {
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 0.0f, 1.0f),
					    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 0.0f, 1.0f),
					    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 1.0f, 1.0f),
					    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 1.0f, 1.0f),
					    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)});

					indices.push_back(index_offset + 0);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 3);
					index_offset += 4;
				}

				// Add east face
				if (i + 1 < w && level.get(i + 1, j) != EntityType::Wall) {
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 0.0f, 0.0f),
					    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 0.0f, 1.0f),
					    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 1.0f, 0.0f),
					    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(1.0f, 1.0f, 1.0f),
					    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});

					indices.push_back(index_offset + 0);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 3);
					index_offset += 4;
				}

				// Add west face
				if (i - 1 >= 0 && level.get(i - 1, j) != EntityType::Wall) {
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 0.0f, 0.0f),
					    glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 0.0f, 1.0f),
					    glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 1.0f, 0.0f),
					    glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
					vertices.push_back(Vertex3DNormTex{
					    offset + glm::vec3(0.0f, 1.0f, 1.0f),
					    glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)});

					indices.push_back(index_offset + 0);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 1);
					indices.push_back(index_offset + 2);
					indices.push_back(index_offset + 3);
					index_offset += 4;
				}
			}
		}
	}
}
