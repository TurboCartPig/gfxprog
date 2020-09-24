/**
 * Assignment 1 - Pacman
 *
 * @author Dennis Kristiansen, NTNU
 * @file pacman.cpp
 */

#include <algorithm>
#include <fstream>
#include <glove/lib.h>
#include <iostream>
#include <memory>

using namespace std::string_literals;

const std::vector<Vertex2D> vertices = {
    Vertex2D{-0.5f, -0.5f}, Vertex2D{-0.5f, 0.5f}, Vertex2D{0.5f, -0.5f},
    Vertex2D{0.5f, 0.5f}};
const std::vector<GLuint> indices = {0, 1, 2, 1, 2, 3};

// Steps:
// 0. Basic setup and init
//    - Create window
//    - Create vertex buffer that can be reused for all the entities
// 1. Load the level
// 2. 2d array where
//    - 0' are tunnel
//    - 1's are walls
//    - 2's are pacman
//    - 3's are ghosts
//    - 4's are pellets
// 3. Draw the level by drawing all the entities individually
//    - Set the transform based on position in level grid

/**
 * The type/variant of entity in the level grid.
 */
enum class EntityType {
	Tunnel = 0,
	Wall   = 1,
	Pacman = 2,
	Ghost  = 3,
	Pellet = 4,
};

/**
 * A level of the game
 */
class Level {
  public:
	/**
	 * Load a single level and initialize the game
	 *
	 * @param path Path to the level file
	 */
	Level(const std::string &path, InputQueue input_queue)
	    : m_input_queue(input_queue) {
		// Load level file from path
		std::ifstream file(path);
		assert(file.good());
		assert(file.is_open());

		// Get level size
		file >> m_width;
		file.ignore();
		file >> m_height;

		m_grid.reserve(m_width * m_height);

		// Loop through cells and push them into grid
		for (size_t i = 0; i < m_width * m_height; i++) {
			int x;
			file >> x;
			m_grid.push_back(static_cast<EntityType>(x));
		}

		// Create vertex buffer and shader program
		m_quad =
		    std::make_unique<VertexBufferObject<Vertex2D>>(vertices, indices);

		const auto paths = {"resources/shaders/pacman.vert"s,
		                    "resources/shaders/pacman.frag"s};
		m_shader_program = std::make_unique<ShaderProgram>(paths);
		m_shader_program->use();

		// Set uniforms
		auto view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
		                        glm::vec3(0.0f, 0.0f, 0.0f),
		                        glm::vec3(0.0f, 1.0f, 0.0));

		auto projection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f);

		m_shader_program->setUniform("u_view", view);
		m_shader_program->setUniform("u_projection", projection);

		// Spawn additional entities into the level grid
		spawnGhosts();
		spawnPellets();
	}

	// FIXME: Draw spritesheets as opposed to static colors
	/**
	 * Draw the entities of the level grid
	 */
	void draw() {
		// Loop through all cells, branch on type, setup uniforms based on type,
		// draw quad
		for (size_t i = 0; i < m_grid.size(); i++) {
			glm::vec3 color;

			// Set some uniforms based on entity type
			switch (m_grid[i]) {
				case EntityType::Wall:
					color = glm::vec3(1.0f, 0.0f, 0.0f);
					break;
				case EntityType::Pacman:
					// TODO: Use pacman spritesheet
					color = glm::vec3(1.0f, 1.0f, 0.0f);
					break;
				case EntityType::Ghost:
					// TODO: Use ghost spritesheet
					color = glm::vec3(1.0f, 1.0f, 1.0f);
					break;
				case EntityType::Pellet:
					// TODO: Use pellet spritesheet or texture
					color = glm::vec3(0.0f, 0.0f, 1.0f);
					break;
				default: continue; // Nothing to draw
			}

			// Set transform to match position in grid
			// FIXME: + 0.5 is a magic number and wont work if level size
			// changes
			auto x = (float)(i % m_width) - m_width / 2.0f + 0.5f;
			auto y = (float)(i / m_width) - m_height / 2.0f + 0.5f;
			auto transform =
			    glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));

			m_shader_program->setUniform("u_color", color);
			m_shader_program->setUniform("u_transform", transform);

			m_quad->draw();
		}
	}

	/**
	 * Drive the simulation forward
	 */
	void update() {
		// Update the state of the grid based on user input

		if (!m_input_queue->empty()) {
			auto pacman_move_dir = m_input_queue->back();
			auto pacman_dirty    = true;

			// FIXME: Pacman can be found multiple times
			// I could find pacman first and then only update that one entity
			// Currently I keep track of whether or not I have updated pacman
			// this frame
			// FIXME: Pacman does not pickup stuff or get killed by ghosts
			for (size_t i = 0; i < m_grid.size(); i++) {
				switch (m_grid[i]) {
					case EntityType::Pacman:
						// Move right
						if (pacman_move_dir == InputCode::D &&
						    m_grid[i + 1] != EntityType::Wall && pacman_dirty) {
							std::swap(m_grid[i], m_grid[i + 1]);
							pacman_dirty = false;
						}
						// Move left
						if (pacman_move_dir == InputCode::A &&
						    m_grid[i - 1] != EntityType::Wall && pacman_dirty) {
							std::swap(m_grid[i - 1], m_grid[i]);
							pacman_dirty = false;
						}
						// Move down
						if (pacman_move_dir == InputCode::S &&
						    m_grid[i - m_width] != EntityType::Wall &&
						    pacman_dirty) {
							std::swap(m_grid[i - m_width], m_grid[i]);
							pacman_dirty = false;
						}
						// Move up
						if (pacman_move_dir == InputCode::W &&
						    m_grid[i + m_width] != EntityType::Wall &&
						    pacman_dirty) {
							std::swap(m_grid[i + m_width], m_grid[i]);
							pacman_dirty = false;
						}
						break;
					case EntityType::Ghost: break;
					default: continue; // Nothing to simulate
				}
			}
		}
	}

	/**
	 * Update state at a regular interval.
	 */
	void fixedUpdate() {}

	// FIXME: Fix randomness
	/**
	 * Spawn ghosts into the level grid
	 */
	void spawnGhosts() {
		std::replace_if(
		    m_grid.begin(), m_grid.end(),
		    [](const EntityType &entity) {
			    return entity == EntityType::Tunnel && rand() % 100 == 0;
		    },
		    EntityType::Ghost);
	}

	// FIXME: Fix randomness
	/**
	 * Spawn pellets into the level grid
	 */
	void spawnPellets() {
		std::replace_if(
		    m_grid.begin(), m_grid.end(),
		    [](const EntityType &entity) {
			    return entity == EntityType::Tunnel && rand() % 100 == 0;
		    },
		    EntityType::Pellet);
	}

	// FIXME: Window should always show the entire grid
	// NOTE: Currently scaling does not work properly
	void setDimensions(const std::pair<int, int> dim) {
		auto [w, h]     = dim;
		auto aspect     = (float)w / (float)h;
		auto invAspect  = (float)h / (float)w;
		auto projection = glm::ortho(
		    -aspect * m_width / 2.0f, aspect * m_width / 2.0f,
		    -invAspect * m_height / 2.0f, invAspect * m_height / 2.0f);
		m_shader_program->setUniform("u_projection", projection);
	}

  private:
	bool                                          m_gameOver = false;
	int                                           m_width;
	int                                           m_height;
	std::vector<EntityType>                       m_grid;
	std::unique_ptr<VertexBufferObject<Vertex2D>> m_quad;
	std::unique_ptr<ShaderProgram>                m_shader_program;
	InputQueue                                    m_input_queue;
};

int main() {
	auto window = Window("Pacman", 900, 900);

	auto level = Level("resources/levels/level0.txt", window.getInputQueue());

	auto dim = window.dimensions();
	level.setDimensions(dim);

	while (!window.pollEvents()) {
		level.update();
		level.draw();

		window.swapBuffers();
	}

	return EXIT_SUCCESS;
}