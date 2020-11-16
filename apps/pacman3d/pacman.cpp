#include "Entities.h"
#include "Level.h"
#include "generation.h"

#include <glove/lib.h>

/**
 * @brief Main game state of pacman 3d.
 */
class GameState : public IGameState {
  public:
	void initialize() override {
		using namespace std::string_literals;

		// Setup framebuffers
		// **********************************************************************************************************
		m_backbuffer = Framebuffer::defaultFramebuffer();

		m_framebuffer = std::make_unique<Framebuffer>();
		m_framebuffer->bind();
		m_framebuffer->addAttachment(AttachmentType::Color,
		                             glm::ivec2(280, 340));
		m_framebuffer->addAttachment(AttachmentType::Depth,
		                             glm::ivec2(280, 340));
		assert(m_framebuffer->valid() && "Minimap framebuffer is not valid!");

		m_shadow_framebuffer = std::make_unique<Framebuffer>();
		m_shadow_framebuffer->bind();
		m_shadow_framebuffer->addAttachment(AttachmentType::Depth,
		                                    glm::ivec2(4096, 4096));
		assert(m_shadow_framebuffer->valid() &&
		       "Shadow map framebuffer is not valid!");

		// Setup opengl state
		// **********************************************************************************************************

		// FIXME: Do this more intelligently
		// TODO: Figure out how to abstract OpenGL state management

		// Enable depth testing
		glEnable(GL_DEPTH_TEST);

		// Enable back face culling.
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Enable blending for transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Load level
		// **********************************************************************************************************
		std::string path = "resources/levels/level0.txt";
		m_level          = std::make_unique<Level>(path);

		// Setup game entities
		// **********************************************************************************************************
		m_maze    = std::make_unique<Maze>(*m_level);
		m_pacman  = std::make_unique<Pacman>(findPacman(*m_level));
		m_pellets = genPellets(*m_level);
		m_ghosts  = genGhosts(*m_level);

		// Load texture
        // **********************************************************************************************************
		m_texture = std::make_unique<Texture>("resources/textures/wall.jpg");

		// Setup shader programs
		// **********************************************************************************************************
		const auto model_shaders = {"resources/shaders/model.vert"s,
		                            "resources/shaders/model.frag"s};
		m_model_shader = std::make_unique<ShaderProgram>(model_shaders);

		const auto pellet_shaders = {"resources/shaders/pellets.vert"s,
		                             "resources/shaders/model.frag"s};
		m_pellet_shader = std::make_unique<ShaderProgram>(pellet_shaders);

		const auto minimap_shaders = {"resources/shaders/model.vert"s,
		                              "resources/shaders/minimap.frag"s};
		m_minimap_shader = std::make_unique<ShaderProgram>(minimap_shaders);

		const auto model_shadow_shaders = {
		    "resources/shaders/model_shadow.vert"s,
		    "resources/shaders/shadow.frag"s};
		m_model_shadow_shader =
		    std::make_unique<ShaderProgram>(model_shadow_shaders);

		const auto pellet_shadow_shaders = {
		    "resources/shaders/pellet_shadow.vert"s,
		    "resources/shaders/shadow.frag"s};
		m_pellet_shadow_shader =
		    std::make_unique<ShaderProgram>(pellet_shadow_shaders);

		// Setup uniforms
		// **********************************************************************************************************
		const auto diffuse_map_slot = 0u;

		m_model_shader->use();
		m_model_shader->setUniform("u_diffuse_map", diffuse_map_slot);

		m_pellet_shader->use();
		m_pellet_shader->setUniform("u_model_color",
		                            glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		m_pellet_shader->setUniform("u_diffuse_map", diffuse_map_slot);
	}

	auto manifest() -> StateManifest override {
		return {"Pacman 3D"};
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
		bool should_quit = false;
		m_pacman->update(dt, *m_level);
		should_quit |= m_pellets->update(*m_pacman);

		for (auto &ghost : m_ghosts) {
			should_quit |= ghost.update(dt, *m_pacman, *m_level);
		}

		if (should_quit)
			return Pop{};
		else
			return None{};
	}

	void render() override {
		// Setup
		// *********************************************************************
		const auto [w, h] = m_level->getSize();

		const auto eye               = glm::vec3(-2.0f, 20.0f, -1.0f);
		const auto target            = glm::vec3(w / 2.0f, 0.0f, h / 2.0f);
		const auto directional_light = DirectionalLight{
		    glm::vec3(1.0f, 1.0f, 1.0f), glm::normalize(eye - target), 2.0f};

		const auto shadow_map_slot = 1u;

		// Zeroth render pass - Generate a shadow map
		// *********************************************************************
		m_shadow_framebuffer->bind();
		m_shadow_framebuffer->clear();

		auto projection = glm::ortho(-23.0f, 23.0f, -23.0f, 23.0f, 5.0f, 50.0f);
		auto view       = glm::lookAt(eye, target, glm::vec3(0.0f, 1.0f, 0.0f));
		auto light_space_matrix = projection * view;

		m_model_shadow_shader->use();
		m_model_shadow_shader->setUniform("u_light_space_matrix",
		                                  light_space_matrix);

		// FIXME: Need to draw pacman as well.
		m_model_shadow_shader->setUniform("u_transform",
		                                  m_maze->getTransform());
		m_maze->draw();

		for (const auto &ghost : m_ghosts) {
			m_model_shadow_shader->setUniform("u_transform",
			                                  ghost.getTransform());
			ghost.draw();
		}

		m_pellet_shadow_shader->use();
		m_pellet_shadow_shader->setUniform("u_light_space_matrix",
		                                   light_space_matrix);

		m_pellets->draw();

		m_shadow_framebuffer->bindDepthAttachmentToSlot(shadow_map_slot);

		// First render pass - Draw scene to the backbuffer
		// *********************************************************************
		m_backbuffer->bind();

		view       = m_pacman->view();
		projection = m_pacman->projection();

		m_model_shader->use();
		m_model_shader->setUniform("u_view", view);
		m_model_shader->setUniform("u_projection", projection);
		m_model_shader->setUniform("u_directional_light", directional_light);
		m_model_shader->setUniform("u_shadow_map", shadow_map_slot);
		m_model_shader->setUniform("u_light_space_matrix", light_space_matrix);

		m_model_shader->setUniform("u_transform", m_maze->getTransform());
		m_model_shader->setUniform("u_model_color",
		                           glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		m_maze->draw();

		m_model_shader->setUniform("u_model_color",
		                           glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		for (const auto &ghost : m_ghosts) {
			m_model_shader->setUniform("u_transform", ghost.getTransform());
			ghost.draw();
		}

		m_pellet_shader->use();
		m_pellet_shader->setUniform("u_view", view);
		m_pellet_shader->setUniform("u_projection", projection);
		m_pellet_shader->setUniform("u_directional_light", directional_light);
		m_pellet_shader->setUniform("u_shadow_map", shadow_map_slot);
		m_pellet_shader->setUniform("u_light_space_matrix", light_space_matrix);

		m_pellets->draw();

		// Second render pass - Draw scene to minimap and blit it to the
		// backbuffer
		// *********************************************************************
		m_framebuffer->bind();
		m_framebuffer->clear();

		view       = glm::lookAt(glm::vec3(0.0f, 10.0f, 0.0f),
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(0.0f, 0.0f, 1.0f));
		projection = glm::ortho(-(float)w, 0.0f, 0.0f, (float)h, 0.0f, 10.0f);

		m_minimap_shader->use();
		m_minimap_shader->setUniform("u_view", view);
		m_minimap_shader->setUniform("u_projection", projection);

		m_minimap_shader->setUniform("u_transform", m_maze->getTransform());
		m_minimap_shader->setUniform("u_model_color",
		                             glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_maze->draw();

		m_minimap_shader->setUniform("u_model_color",
		                             glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		for (const auto &ghost : m_ghosts) {
			m_minimap_shader->setUniform("u_transform", ghost.getTransform());
			ghost.draw();
		}

		m_pellet_shader->use();
		m_pellet_shader->setUniform("u_view", view);
		m_pellet_shader->setUniform("u_projection", projection);

		m_pellets->draw();

		m_backbuffer->blit(m_framebuffer.get(), glm::ivec4(0, 0, 280, 340),
		                   glm::ivec4(0, 0, 280, 340));

		// Render pass end
		// *********************************************************************

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
	    m_model_shader; ///< Default model shader program (Used for e.g. the
	                    ///< maze).
	std::unique_ptr<ShaderProgram>
	    m_pellet_shader; ///< Shader used for drawing pellets instanced.
	std::unique_ptr<ShaderProgram>
	    m_minimap_shader; ///< Minimap shader program.
	std::unique_ptr<ShaderProgram>
	    m_model_shadow_shader; ///< Shadow map generating shader program.
	std::unique_ptr<ShaderProgram>
	    m_pellet_shadow_shader; ///< Shadow map generating shader program.

	std::unique_ptr<Texture> m_texture; ///< A texture for the walls.

	std::unique_ptr<Framebuffer>
	    m_backbuffer; ///< Default framebuffer created by GLFW.
	std::unique_ptr<Framebuffer>
	    m_framebuffer; ///< Framebuffer for drawing the minimap to.
	std::unique_ptr<Framebuffer>
	    m_shadow_framebuffer; ///< Framebuffer for generating a shadow map.
};

auto main() -> int {
	// Manage game state via pushdown automata
	auto core = Core(std::make_unique<GameState>());
	core.run();

	return 0;
}
