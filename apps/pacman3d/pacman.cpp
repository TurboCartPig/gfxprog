#include "Entities.h"
#include "Level.h"

#include <glove/lib.h>

/**
 * @brief Main game state of pacman 3d.
 */
class GameState : public IGameState {
  public:
	void initialize() override {
		using namespace std::string_literals;

		// Setup opengl state
		// FIXME: Do this more intelligently

		m_backbuffer = Framebuffer::defaultFramebuffer();

		m_framebuffer = std::make_unique<Framebuffer>();
		m_framebuffer->bind();
		m_framebuffer->addAttachment(AttachmentType::Color,
		                             glm::ivec2(240, 240));
		m_framebuffer->addAttachment(AttachmentType::DepthStencil,
		                             glm::ivec2(240, 240));
		assert(m_framebuffer->valid());

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

		// Setup shader programs
		const auto model_shaders = {"resources/shaders/model.vert"s,
		                      "resources/shaders/model.frag"s};
		m_model_shader     = std::make_unique<ShaderProgram>(model_shaders);

        const auto minimap_shaders = {"resources/shaders/model.vert"s,
                              "resources/shaders/minimap.frag"s};
        m_minimap_shader     = std::make_unique<ShaderProgram>(minimap_shaders);

		// Setup uniforms
		auto transform = glm::mat4(1.0f);

		auto model_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		auto directional_light = DirectionalLight{
		    glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.25f, 1.0f, 0.5f), 2.0f};

		m_model_shader->use();
		m_model_shader->setUniform("u_transform", transform);
		m_model_shader->setUniform("u_model_color", model_color);
		m_model_shader->setUniform("u_directional_light", directional_light);
		m_model_shader->setUniform("u_diffuse_map", 0u);

		m_minimap_shader->use();
		m_minimap_shader->setUniform("u_transform", transform);
	}

	auto input(Input input) -> StateTransition override {
		if (input.state == InputState::Pressed) {
			if (input.code == InputCode::Escape)
				return Pop{};
		}

		m_pacman->input(input);

		return None{};
	}

	auto update(float dt) -> StateTransition override {
		m_pacman->update(dt, *m_level);
		m_pellets->update(*m_pacman);

		return None{};
	}

	void render() override {
		// First render pass - Draw scene to the backbuffer
		// **********************************************************************************************************
		m_backbuffer->bind();

		auto view       = m_pacman->view();
		auto projection = m_pacman->projection();

		m_model_shader->use();
		m_model_shader->setUniform("u_view", view);
		m_model_shader->setUniform("u_projection", projection);

		m_maze->draw();
		m_pellets->draw(view, projection);

		// for (auto &ghost : m_ghosts) {
		// 	ghost.draw();
		// }

		// Second render pass - Draw scene to minimap and blit it to the backbuffer
        // **********************************************************************************************************
		m_framebuffer->bind();
		m_framebuffer->clear();

		glViewport(0, 0, 240, 240);

		// Draw stuff
		auto [w, h] = m_level->getSize();
		view        = glm::lookAt(glm::vec3(0.0f, 10.0f, 0.0f),
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(0.0f, 0.0f, 1.0f));
		projection  = glm::ortho(-(float)w, 0.0f, 0.0f, (float)h, 0.0f, 10.0f);

		m_minimap_shader->use();
		m_minimap_shader->setUniform("u_view", view);
		m_minimap_shader->setUniform("u_projection", projection);
		m_minimap_shader->setUniform("u_model_color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		m_maze->draw();
		m_pellets->draw(view, projection); // FIXME: Still uses it's own shader

		m_backbuffer->blit(m_framebuffer.get(), glm::ivec4(0, 0, 240, 240),
		                   glm::ivec4(0, 0, 240, 240));

		glViewport(0, 0, 1280, 720);

		// Render pass end
        // **********************************************************************************************************

		// Bind the backbuffer for GLFW to read from
		m_backbuffer->bind();
	}

  private:
	std::unique_ptr<Level>   m_level;   ///< The current level.
	std::unique_ptr<Maze>    m_maze;    ///< The level maze.
	std::unique_ptr<Pacman>  m_pacman;  ///< Pacman entity.
	std::unique_ptr<Pellets> m_pellets; ///< All the pellets in the level.
	std::vector<Ghost>       m_ghosts;  ///< All the ghosts in the level.

	std::unique_ptr<ShaderProgram>
	    m_model_shader; ///< Default model shader program (Used for e.g. the maze).
    std::unique_ptr<ShaderProgram>
        m_minimap_shader; ///< Minimap shader program.
	std::unique_ptr<Framebuffer> m_framebuffer;
	std::unique_ptr<Framebuffer>
	    m_backbuffer; ///< Default framebuffer created by GLFW.
};

auto main() -> int {
	// Manage game state via pushdown automata
	auto core = Core(std::make_unique<GameState>());
	core.run();

	return 0;
}
