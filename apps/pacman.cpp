/**
 * Assignment 1 - Pacman
 *
 * @author Dennis Kristiansen, NTNU
 * @file pacman.cpp
 */

#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <glove/lib.h>
#include <iostream>
#include <memory>
#include <random>
#include <utility>

using namespace std::string_literals;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

const std::vector<Vertex2DTex> vertices = {
    Vertex2DTex{-0.5f, -0.5f, 1.0f, 1.0f}, Vertex2DTex{-0.5f, 0.5f, 1.0f, 0.0f},
    Vertex2DTex{0.5f, -0.5f, 0.0f, 1.0f}, Vertex2DTex{0.5f, 0.5f, 0.0f, 0.0f}};
const std::vector<GLuint> indices = {0, 1, 2, 1, 2, 3};

std::function<float()> rnd; ///< Lazily bound function to get random numbers

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
 * Direction of movement.
 */
enum class Direction {
	Left,
	Right,
	Up,
	Down,
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
	 * @param input_queue Input queue from window
	 */
	Level(const std::string &path, InputQueue input_queue)
	    : m_input_queue(std::move(input_queue)) {
		// Load level file from path
		std::ifstream file(path);
		assert(file.good());
		assert(file.is_open());

		// Get level size
		file >> m_width;
		file.ignore();
		file >> m_height;

		m_grid.resize(m_width * m_height);

		// Loop through cells and push them into grid
		// FIXME: Level is fliped around the x axis
		for (size_t i = 0; i < m_width * m_height; i++) {
			int x;
			file >> x;
			m_grid[i] = static_cast<EntityType>(x);
		}

		// Enable blending for transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		// Create vertex buffer
		m_quad = std::make_unique<VertexBufferObject<Vertex2DTex>>(vertices,
		                                                           indices);

		// Create shader program
		const auto paths = {"resources/shaders/pacman.vert"s,
		                    "resources/shaders/pacman.frag"s};
		m_shader_program = std::make_unique<ShaderProgram>(paths);
		m_shader_program->use();

		// Set default uniforms
		auto view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
		                        glm::vec3(0.0f, 0.0f, 0.0f),
		                        glm::vec3(0.0f, 1.0f, 0.0));

		auto projection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f);

		auto sprite_sheet_location = 0u;

		m_shader_program->setUniform("u_view", view);
		m_shader_program->setUniform("u_projection", projection);
		m_shader_program->setUniform("u_sprite_sheet", sprite_sheet_location);

		// Load shared sprite sheet
		auto spritesheet = std::make_shared<Texture>(
		    "resources/textures/pacman.png", GL_TEXTURE_RECTANGLE);
		spritesheet->bindToSlot(sprite_sheet_location);

		// Define animations
		// FIXME: Animations are not correct
		// FIXME: Pixels overlap
		SpriteSheetAnimation pacman_up_anim;
		pacman_up_anim.pushKeyFrame(glm::ivec4(0, 72, 72, 144));
		pacman_up_anim.pushKeyFrame(glm::ivec4(72, 72, 144, 144));
		pacman_up_anim.pushKeyFrame(glm::ivec4(144, 72, 216, 144));
		pacman_up_anim.pushKeyFrame(glm::ivec4(216, 72, 288, 144));
		pacman_up_anim.pushKeyFrame(glm::ivec4(144, 72, 216, 144));
		pacman_up_anim.pushKeyFrame(glm::ivec4(72, 72, 144, 144));

		SpriteSheetAnimation pacman_down_anim;
		pacman_down_anim.pushKeyFrame(glm::ivec4(0, 0, 72, 72));
		pacman_down_anim.pushKeyFrame(glm::ivec4(72, 0, 144, 72));
		pacman_down_anim.pushKeyFrame(glm::ivec4(144, 0, 216, 72));
		pacman_down_anim.pushKeyFrame(glm::ivec4(216, 0, 288, 72));
		pacman_down_anim.pushKeyFrame(glm::ivec4(144, 0, 216, 72));
		pacman_down_anim.pushKeyFrame(glm::ivec4(72, 0, 144, 72));

		SpriteSheetAnimation pacman_left_anim;
		pacman_left_anim.pushKeyFrame(glm::ivec4(0, 216, 72, 288));
		pacman_left_anim.pushKeyFrame(glm::ivec4(72, 216, 144, 288));
		pacman_left_anim.pushKeyFrame(glm::ivec4(144, 216, 216, 288));
		pacman_left_anim.pushKeyFrame(glm::ivec4(216, 216, 288, 288));
		pacman_left_anim.pushKeyFrame(glm::ivec4(144, 216, 216, 288));
		pacman_left_anim.pushKeyFrame(glm::ivec4(72, 216, 144, 288));

		SpriteSheetAnimation pacman_right_anim;
		pacman_right_anim.pushKeyFrame(glm::ivec4(0, 144, 72, 216));
		pacman_right_anim.pushKeyFrame(glm::ivec4(72, 144, 144, 216));
		pacman_right_anim.pushKeyFrame(glm::ivec4(144, 144, 216, 216));
		pacman_right_anim.pushKeyFrame(glm::ivec4(216, 144, 288, 216));
		pacman_right_anim.pushKeyFrame(glm::ivec4(144, 144, 216, 216));
		pacman_right_anim.pushKeyFrame(glm::ivec4(72, 144, 144, 216));

		// Put animations in a map with pacman's movement directions as the keys
		m_pacman_animations.insert(
		    std::make_pair(Direction::Up, pacman_up_anim));
		m_pacman_animations.insert(
		    std::make_pair(Direction::Down, pacman_down_anim));
		m_pacman_animations.insert(
		    std::make_pair(Direction::Right, pacman_right_anim));
		m_pacman_animations.insert(
		    std::make_pair(Direction::Left, pacman_left_anim));

		// Pacman's animations
		m_pacman_animation = std::make_unique<AnimatedSpriteSheet>(
		    milliseconds(50), m_pacman_animations[Direction::Right],
		    spritesheet);

		// Ghosts' animations
		// TODO: Animate ghosts

		// Spawn additional entities into the level grid
		spawnGhosts();
		spawnPellets();
	}

	/**
	 * Draw the entities of the level grid
	 */
	void draw() {
		// Loop through all cells, branch on type, setup uniforms based on type,
		// draw quad
		for (size_t i = 0; i < m_grid.size(); i++) {
			if (m_grid[i] == EntityType::Tunnel)
				continue; // Don't draw tunnels

			// If alpha is ~0.0, sample sprite sheet instead.
			// Otherwise use color value.
			glm::vec4 color = glm::vec4(0.0f);

			// Set transform to match position in grid
			auto x = i % m_width - m_width / 2.0f + 0.5f;
			auto y = i / m_width - m_height / 2.0f + 0.5f;
			auto transform =
			    glm::translate(glm::mat4(1.0f), glm::vec3(x, -y, 0.0f));

			// Set some uniforms based on entity type
			switch (m_grid[i]) {
				case EntityType::Wall:
					color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
					break;
				case EntityType::Pacman:
					// FIXME: Fractional positioning doesn't work
					//					transform = glm::translate(transform,
					//					                           glm::vec3(m_pacman_delta,
					//0.0f));
					break;
				case EntityType::Ghost:
					color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
					break;
				case EntityType::Pellet:
					color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
					transform =
					    glm::scale(transform, glm::vec3(0.25f, 0.25f, 1.0f));
					transform = glm::rotate(transform, (float)M_PI / 4.0f,
					                        glm::vec3(0.0f, 0.0f, 1.0f));
					break;
				default: break; // No special uniforms to set
			}

			m_shader_program->setUniform("u_color", color);
			m_shader_program->setUniform("u_transform", transform);
			m_shader_program->setUniform("u_key_frame",
			                             m_pacman_animation->getUniform());

			m_quad->draw();
		}
	}

	/**
	 * Drive the simulation forward
	 */
	void update(std::chrono::duration<double> dt) {
		// Update pacman's movement direction based on user input
		if (!m_input_queue->empty()) {
			auto input = m_input_queue->back();

			switch (input) {
				case InputCode::W: m_pacman_move_dir = Direction::Up; break;
				case InputCode::S: m_pacman_move_dir = Direction::Down; break;
				case InputCode::A: m_pacman_move_dir = Direction::Left; break;
				case InputCode::D: m_pacman_move_dir = Direction::Right; break;
				default: break;
			}

			// Change pacman's animation
			m_pacman_animation->playAnimation(
			    m_pacman_animations.at(m_pacman_move_dir));
		}

		// Update fractional part of pacman's position
		const float delta = dt.count() * 2.0f;
		switch (m_pacman_move_dir) {
			case Direction::Up: m_pacman_delta.y += delta; break;
			case Direction::Down: m_pacman_delta.y -= delta; break;
			case Direction::Right: m_pacman_delta.x += delta; break;
			case Direction::Left: m_pacman_delta.x -= delta; break;
		}

		m_pacman_animation->update(dt);
	}

	/**
	 * Update state at a regular interval.
	 */
	void fixedUpdate() {
		// Find pacman and update his position
		size_t pacman_pos;
		for (size_t i = 0; i < m_grid.size(); i++) {
			if (m_grid[i] == EntityType::Pacman) {
				pacman_pos = i;
			}
		}

		// Move right
		if (m_pacman_move_dir == Direction::Right) {
			movePacman(pacman_pos, pacman_pos + 1);
		}
		// Move left
		else if (m_pacman_move_dir == Direction::Left) {
			movePacman(pacman_pos, pacman_pos - 1);
		}
		// Move up
		else if (m_pacman_move_dir == Direction::Up) {
			movePacman(pacman_pos, pacman_pos - m_width);
		}
		// Move down
		else if (m_pacman_move_dir == Direction::Down) {
			movePacman(pacman_pos, pacman_pos + m_width);
		}

		// Reset fractional part
		m_pacman_delta = glm::vec2(0.0f);
	}

	/**
	 * Move pacman from old_pos to new_pos while respecting game rules.
	 *
	 * @param old_pos Pacman's old position
	 * @param new_pos Pacman's new position
	 */
	void movePacman(const size_t old_pos, const size_t new_pos) {
		if (m_grid[new_pos] != EntityType::Wall) {
			if (m_grid[new_pos] == EntityType::Pellet) {
				pickupPellet();
			}

			if (m_grid[new_pos] == EntityType::Ghost) {
				m_grid[new_pos] = EntityType::Ghost;
				endGame();
			} else {
				m_grid[new_pos] = EntityType::Pacman;
			}

			m_grid[old_pos] = EntityType::Tunnel;
		}
	}

	/**
	 * Pickup one pellet.
	 */
	void pickupPellet() {
		if (++m_pellet_count >= m_total_pellets) {
			std::cout << "Game won!" << std::endl;
			m_gameWon = true;
		}
	}

	/**
	 * End the game.
	 */
	void endGame() {
		std::cout << "Game Over" << std::endl;
		m_gameOver = true;
	}

	/**
	 * Spawn ghosts into the level grid
	 */
	void spawnGhosts() {
		std::replace_if(
		    m_grid.begin(), m_grid.end(),
		    [](const EntityType &entity) {
			    return entity == EntityType::Tunnel && rnd() <= 0.02f;
		    },
		    EntityType::Ghost);
	}

	/**
	 * Spawn pellets into the level grid
	 */
	void spawnPellets() {
		int pellet_count = 0;
		std::replace_if(
		    m_grid.begin(), m_grid.end(),
		    [&](const EntityType &entity) {
			    pellet_count++;
			    return entity == EntityType::Tunnel;
		    },
		    EntityType::Pellet);

		m_total_pellets = pellet_count;
	}

	// FIXME: Window should always show the entire grid
	void setDimensions(const std::pair<int, int> dim) {
		auto [w, h] = dim;
		auto aspect = (float)w / (float)h;
		auto projection =
		    glm::ortho(-aspect * m_width / 2.0f, aspect * m_width / 2.0f,
		               -m_height / 2.0f, m_height / 2.0f);
		m_shader_program->setUniform("u_projection", projection);
	}

	/**
	 * Is the game over?
	 *
	 * @return true if the game is over
	 */
	bool gameOver() const { return m_gameOver || m_gameWon; }

  private:
	bool m_gameOver     = false;
	bool m_gameWon      = false;
	int  m_pellet_count = 0;
	int  m_total_pellets;

	int m_width;
	int m_height;

	InputQueue m_input_queue;

	std::vector<EntityType>                          m_grid;
	std::unique_ptr<VertexBufferObject<Vertex2DTex>> m_quad;
	std::unique_ptr<ShaderProgram>                   m_shader_program;

	// Pacman's stuff
	glm::vec2 m_pacman_delta; // Fractional position delta
	Direction m_pacman_move_dir = Direction::Right;
	std::unique_ptr<AnimatedSpriteSheet>                m_pacman_animation;
	std::unordered_map<Direction, SpriteSheetAnimation> m_pacman_animations;

	// Ghosts' stuff
	// FIXME: Do we have to keep track of this per ghost?
	std::unique_ptr<AnimatedSpriteSheet>                m_ghost_animation;
	std::unordered_map<Direction, SpriteSheetAnimation> m_ghost_animations;
};

int main() {
	// Init randomness
	std::random_device                    rd;
	std::default_random_engine            generator(rd());
	std::uniform_real_distribution<float> distribution(0.0, 1.0);
	rnd = std::bind(distribution, generator);

	// Init window
	auto window = Window("Pacman", 900, 900);

	auto level = Level("resources/levels/level0.txt", window.getInputQueue());

	// Setup clock for time measurements
	auto tick_start = steady_clock::now();
	auto prev       = steady_clock::now();

	// Main game loop
	while (!window.pollEvents()) {
		// Game over, but waiting to close
		if (!level.gameOver()) {
			const auto                          now = steady_clock::now();
			const std::chrono::duration<double> dt  = (now - prev);
			prev                                    = now;

			level.update(dt);

			// Run fixed update at fixed interval
			if ((now - tick_start) > milliseconds(500)) {
				tick_start = now;
				level.fixedUpdate();
			}
		}

		level.draw();

		window.swapBuffers();
		auto dim = window.dimensions();
		level.setDimensions(dim);
	}

	return EXIT_SUCCESS;
}
