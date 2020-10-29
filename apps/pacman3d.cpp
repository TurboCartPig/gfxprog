#include <cassert>
#include <fstream>
#include <glove/lib.h>

using namespace std::string_literals;

/**
 * The type/variant of entity in the level grid.
 */
enum class EntityType : uint32_t {
	Tunnel = 0,
	Wall   = 1,
	Pacman = 2,
	Ghost  = 3,
	Pellet = 4,
};

/**
 * Abstract level representation.
 */
class Level {
  public:
	Level(const std::string &path) {
		// Load level file from path
		std::ifstream file(path);
		assert(file.good());
		assert(file.is_open());

		// Get level size
		file >> m_width;
		file.ignore();
		file >> m_height;

		m_grid.resize(m_width * m_height);

		// Load level file into grid
		for (auto &cell : m_grid) {
			int x;
			file >> x;
			cell = static_cast<EntityType>(x);
		}
	}

	/**
	 * Get the type of entity at the given x,y coordinate.
	 * @param x X coordinate / width index.
	 * @param y T coordinate / height index.
	 * @return Type of entity in the level at give coordinates.
	 */
	EntityType get(int x, int y) const { return m_grid[y * m_width + x]; }

	/**
	 * Get the size of the level with entities as units.
	 * @return Width / Height of the level.
	 */
	std::pair<int, int> getSize() const {
		return std::make_pair(m_width, m_height);
	}

  private:
	int                     m_width;
	int                     m_height;
	std::vector<EntityType> m_grid;
};

void genLevelMesh(const Level &level, std::vector<Vertex3DNormTex> &vertices,
                  std::vector<uint32_t> &indices) {
	auto [w, h] = level.getSize();
	for (int j = 0; j < h; ++j) {
		for (int i = 0; i < w; ++i) {
			const auto entity = level.get(i, j);
			if (entity == EntityType::Wall) {
				const auto offset       = glm::vec3(i, 0.0f, j);
				const auto index_offset = vertices.size();

				// Add 4 * 6 = 24 vertices
				// Add bottom face
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(0.0f, 0.0f, 0.0f),
				    glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(0.0f, 0.0f, 1.0f),
				    glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(1.0f, 0.0f, 0.0f),
				    glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});
				vertices.push_back(Vertex3DNormTex{
				    offset + glm::vec3(1.0f, 0.0f, 1.0f),
				    glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)});

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

				// Add 36 indices
				// Add bottom face
				indices.push_back(index_offset + 0);
				indices.push_back(index_offset + 1);
				indices.push_back(index_offset + 2);
				indices.push_back(index_offset + 1);
				indices.push_back(index_offset + 2);
				indices.push_back(index_offset + 3);

                // Add top face
                indices.push_back(index_offset + 4);
                indices.push_back(index_offset + 5);
                indices.push_back(index_offset + 6);
                indices.push_back(index_offset + 5);
                indices.push_back(index_offset + 6);
                indices.push_back(index_offset + 7);
			}
		}
	}
}

/**
 * Main game state of pacman 3d.
 */
class GameState : public IGameState {
  public:
	GameState() = default;

	void initialize() override {
		// Load level
		auto level = Level("resources/levels/level0.txt");

		// Turn level into mesh
		std::vector<Vertex3DNormTex> level_mesh_vertices;
		std::vector<uint32_t>        level_mesh_indices;
		genLevelMesh(level, level_mesh_vertices, level_mesh_indices);

		m_level_mesh = std::make_unique<VertexBuffer<Vertex3DNormTex>>(
		    level_mesh_vertices, level_mesh_indices);

		// Setup rendering
		const auto shaders = {"resources/shaders/model.vert"s,
		                      "resources/shaders/model.frag"s};
		m_shader_program   = std::make_unique<ShaderProgram>(shaders);
		m_shader_program->use();

		auto transform = glm::mat4(1.0f);

		glm::quat q = glm::quatLookAt(glm::vec3(0.0f, 0.0f, -1.0f),
		                              glm::vec3(0.0f, 1.0f, 0.0f));
		m_transform = {glm::vec3(0.0f, 1.0f, -2.0f), q, glm::vec3(1.0f)};
		m_camera = CameraComponent(16.0f / 9.0f, 96.0f);
		auto view_projection = m_camera.asViewProjection(m_transform);

		auto model_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		auto directional_light = DirectionalLight{
		    glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.5f), 10.0f};

		m_shader_program->setUniform("u_transform", transform);
		m_shader_program->setUniform("u_view_projection", view_projection);
		m_shader_program->setUniform("u_model_color", model_color);
		m_shader_program->setUniform("u_directional_light", directional_light);
		m_shader_program->setUniform("u_diffuse_map", 0u);

		// Deal with collision setup
	}

	[[nodiscard]] StateTransition input(InputCode code) override {
		if (code == InputCode::Escape)
			return Pop{};
		else if (code == InputCode::W) {
			m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
		} else if (code == InputCode::S) {
			m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
		} else if (code == InputCode::A) {
			m_forward = glm::vec3(1.0f, 0.0f, 0.0f);
		} else if (code == InputCode::D) {
			m_forward = glm::vec3(-1.0f, 0.0f, 0.0f);
		}

		return None{};
	}

	[[nodiscard]] StateTransition update(float dt) override {
		m_transform.translation += m_forward * dt;

		return None{};
	}

	void render() override {
        auto view_projection = m_camera.asViewProjection(m_transform);

		m_shader_program->use();
        m_shader_program->setUniform("u_view_projection", view_projection);
		m_level_mesh->draw();
	}

  private:
	//  Pacman m_pacman;
	//	std::vector<Ghost> m_ghosts;

	// Camera stuff
	glm::vec3          m_forward;
	TransformComponent m_transform;
	CameraComponent    m_camera;

	std::unique_ptr<VertexBuffer<Vertex3DNormTex>> m_level_mesh;
	std::unique_ptr<ShaderProgram>                 m_shader_program;
};

int main() {
	auto core = Core(std::make_unique<GameState>());
	core.run();

	return 0;
}