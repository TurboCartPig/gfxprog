#include "Entities3d.h"

#include <fstream>
#include <glove/lib.h>
#include <unordered_map>

using namespace std::string_literals;

/**
 * Main game state of pacman 3d.
 */
class GameState : public IGameState {
  public:
	GameState() = default;

	void initialize() override {
		// Setup opengl state
		// FIXME: Do this more intelligently
		// Enable depth testing
		glEnable(GL_DEPTH_TEST);

		// Enable blending for transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		// Load level
		auto level = Level("resources/levels/level0.txt");

		m_maze = std::make_unique<Maze>(level);

		// Setup rendering
		const auto shaders = {"resources/shaders/model.vert"s,
		                      "resources/shaders/model.frag"s};
		m_shader_program   = std::make_unique<ShaderProgram>(shaders);
		m_shader_program->use();

		auto transform = glm::mat4(1.0f);

		glm::quat q = glm::quatLookAt(glm::vec3(0.0f, 0.0f, -1.0f),
		                              glm::vec3(0.0f, 1.0f, 0.0f));
		m_transform = {glm::vec3(0.0f, 0.5f, -2.0f), q, glm::vec3(1.0f)};
		m_camera    = CameraComponent(16.0f / 9.0f, 96.0f);
		auto view_projection = m_camera.asViewProjection(m_transform);

		auto model_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		auto directional_light = DirectionalLight{
		    glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.25f, 1.0f, 0.5f), 10.0f};

		m_shader_program->setUniform("u_transform", transform);
		m_shader_program->setUniform("u_view_projection", view_projection);
		m_shader_program->setUniform("u_model_color", model_color);
		m_shader_program->setUniform("u_directional_light", directional_light);
		m_shader_program->setUniform("u_diffuse_map", 0u);

		// Deal with collision setup
	}

	[[nodiscard]] StateTransition input(Input input) override {
		if (input.state == InputState::Pressed) {
			if (input.code == InputCode::Escape)
				return Pop{};
			else if (input.code == InputCode::W) {
				m_forward.z = 1.0f;
			} else if (input.code == InputCode::S) {
				m_forward.z = -1.0f;
			} else if (input.code == InputCode::A) {
				m_forward.x = 1.0f;
			} else if (input.code == InputCode::D) {
				m_forward.x = -1.0f;
			}
		} else if (input.state == InputState::Released) {
			if (input.code == InputCode::W || input.code == InputCode::S)
				m_forward.z = 0.0f;
			else if (input.code == InputCode::A || input.code == InputCode::D)
				m_forward.x = 0.0f;
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
		m_maze->draw();

		// m_pacman->update();

		// for (auto &ghost : m_ghosts) {
		// 	ghost.update();
		// 	ghost.draw();
		// }

		// for (const auto &pellet : m_pellets) {
		// 	pellet.draw();
		// }
	}

  private:
	std::unique_ptr<Maze>   m_maze;
	std::unique_ptr<Pacman> m_pacman;
	std::vector<Ghost>      m_ghosts;
	std::vector<Pellet>     m_pellets;

	// Camera stuff
	glm::vec3          m_forward;
	TransformComponent m_transform;
	CameraComponent    m_camera;

	std::unique_ptr<ShaderProgram> m_shader_program;
};

int main() {
	auto core = Core(std::make_unique<GameState>());
	core.run();

	return 0;
}
