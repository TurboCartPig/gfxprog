#include "Entities.h"
#include "Level.h"

#include <fstream>
#include <glove/lib.h>
#include <unordered_map>

/**
 * Main game state of pacman 3d.
 */
class GameState : public IGameState {
  public:
	GameState() = default;

	void initialize() override {
		using namespace std::string_literals;

		// Setup opengl state
		// FIXME: Do this more intelligently

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);

		// Enable blending for transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		// Load level
		std::string path = "resources/levels/level0.txt";
		m_level          = std::make_unique<Level>(path);

		// Setup game entities
		m_maze    = std::make_unique<Maze>(*m_level);
		m_pacman  = std::make_unique<Pacman>(findPacman(*m_level));
		m_pellets = genPellets(*m_level);
		m_ghosts  = genGhosts(*m_level);

		// Setup default shader program
		const auto shaders = {"resources/shaders/model.vert"s,
		                      "resources/shaders/model.frag"s};
		m_shader_program   = std::make_unique<ShaderProgram>(shaders);
		m_shader_program->use();

		auto transform = glm::mat4(1.0f);

		auto model_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		auto directional_light = DirectionalLight{
		    glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.25f, 1.0f, 0.5f), 2.0f};

		m_shader_program->setUniform("u_transform", transform);
		m_shader_program->setUniform("u_model_color", model_color);
		m_shader_program->setUniform("u_directional_light", directional_light);
		m_shader_program->setUniform("u_diffuse_map", 0u);
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
		m_pacman->update(dt, *m_level);
		m_pellets->update(*m_pacman);

		return None{};
	}

	void render() override {
		auto view       = m_pacman->view();
		auto projection = m_pacman->projection();

		m_shader_program->use();
		m_shader_program->setUniform("u_view", view);
		m_shader_program->setUniform("u_projection", projection);

		m_maze->draw();
		m_pellets->draw(view, projection);

		// for (auto &ghost : m_ghosts) {
		// 	ghost.draw();
		// }
	}

  private:
	std::unique_ptr<Level>   m_level;   ///< The current level.
	std::unique_ptr<Maze>    m_maze;    ///< The level maze.
	std::unique_ptr<Pacman>  m_pacman;  ///< Pacman entity.
	std::unique_ptr<Pellets> m_pellets; ///< All the pellets in the level.
	std::vector<Ghost>       m_ghosts;  ///< All the ghosts in the level.

	std::unique_ptr<ShaderProgram>
	    m_shader_program; ///< Default shader program (Used for the maze)
};

int main() {
	// Manage game state via pushdown automata
	auto core = Core(std::make_unique<GameState>());
	core.run();

	return 0;
}
