#include "Entities3d.h"
#include "Level3d.h"

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
		m_pacman  = std::make_unique<Pacman>(findPacman(level));
		m_ghosts  = genGhosts(level);
		m_pellets = genPellets(level);

		// Setup rendering
		const auto shaders = {"resources/shaders/model.vert"s,
		                      "resources/shaders/model.frag"s};
		m_shader_program   = std::make_unique<ShaderProgram>(shaders);
		m_shader_program->use();

		auto transform = glm::mat4(1.0f);

		auto model_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		auto directional_light = DirectionalLight{
		    glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.25f, 1.0f, 0.5f), 10.0f};

		m_shader_program->setUniform("u_transform", transform);
		m_shader_program->setUniform("u_model_color", model_color);
		m_shader_program->setUniform("u_directional_light", directional_light);
		m_shader_program->setUniform("u_diffuse_map", 0u);

		// Deal with collision setup
	}

	[[nodiscard]] StateTransition input(Input input) override {
		if (input.state == InputState::Pressed) {
			if (input.code == InputCode::Escape)
				return Pop{};
		}

		m_pacman->input(input);

		return None{};
	}

	[[nodiscard]] StateTransition update(float dt) override {
		m_pacman->update(dt);
		return None{};
	}

	void render() override {
		auto view_projection = m_pacman->viewProjection();

		m_shader_program->use();
		m_shader_program->setUniform("u_view_projection", view_projection);
		m_maze->draw();

		// for (auto &ghost : m_ghosts) {
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

	std::unique_ptr<ShaderProgram> m_shader_program;
};

int main() {
	auto core = Core(std::make_unique<GameState>());
	core.run();

	return 0;
}
