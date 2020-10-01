/**
 * Assignment 1 - Pacman
 *
 * @author Dennis Kristiansen, NTNU
 * @file pacman.cpp
 */

#include "Entities.h"

#include <chrono>
#include <fstream>
#include <functional>
#include <glove/lib.h>
#include <iostream>
#include <memory>
#include <random>
#include <utility>
#include <variant>

using namespace std::string_literals;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

const size_t                   NUM_GHOSTS = 4;
const std::vector<Vertex2DTex> vertices   = {
    Vertex2DTex{0.0f, 0.0f, 1.0f, 1.0f}, Vertex2DTex{0.0f, 1.0f, 1.0f, 0.0f},
    Vertex2DTex{1.0f, 0.0f, 0.0f, 1.0f}, Vertex2DTex{1.0f, 1.0f, 0.0f, 0.0f}};
// FIXME: Actually make this a circle
const std::vector<Vertex2DTex> circle_vertices = {
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

std::unique_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
create_pacman_animations() {
	// Define animations
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
	auto pacman_animations =
	    std::make_unique<std::unordered_map<Direction, SpriteSheetAnimation>>();
	pacman_animations->insert(std::make_pair(Direction::Up, pacman_up_anim));
	pacman_animations->insert(
	    std::make_pair(Direction::Down, pacman_down_anim));
	pacman_animations->insert(
	    std::make_pair(Direction::Right, pacman_right_anim));
	pacman_animations->insert(
	    std::make_pair(Direction::Left, pacman_left_anim));

	return pacman_animations;
}

std::shared_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
create_ghost_animations() {
	// Define animations
	SpriteSheetAnimation ghost_up_anim;
	ghost_up_anim.pushKeyFrame(glm::ivec4(288, 72, 360, 144));
	ghost_up_anim.pushKeyFrame(glm::ivec4(360, 72, 432, 144));

	SpriteSheetAnimation ghost_down_anim;
	ghost_down_anim.pushKeyFrame(glm::ivec4(288, 0, 360, 72));
	ghost_down_anim.pushKeyFrame(glm::ivec4(360, 0, 432, 72));

	SpriteSheetAnimation ghost_right_anim;
	ghost_right_anim.pushKeyFrame(glm::ivec4(288, 216, 360, 288));
	ghost_right_anim.pushKeyFrame(glm::ivec4(360, 216, 432, 288));

	SpriteSheetAnimation ghost_left_anim;
	ghost_left_anim.pushKeyFrame(glm::ivec4(288, 144, 360, 216));
	ghost_left_anim.pushKeyFrame(glm::ivec4(360, 144, 432, 216));

	// Store animations in a map
	auto ghost_animations =
	    std::make_shared<std::unordered_map<Direction, SpriteSheetAnimation>>();
	ghost_animations->insert(std::make_pair(Direction::Up, ghost_up_anim));
	ghost_animations->insert(std::make_pair(Direction::Down, ghost_down_anim));
	ghost_animations->insert(
	    std::make_pair(Direction::Right, ghost_right_anim));
	ghost_animations->insert(std::make_pair(Direction::Left, ghost_left_anim));

	return ghost_animations;
}

/**
 * A level of the game
 */
class Level {
  public:
	/**
	 * Load a single level and initialize the game
	 * @param path Path to the level file
	 * @param input_queue Input queue from window
	 */
	Level(const std::string &path, InputQueue input_queue)
	    : m_input_queue(std::move(input_queue)) {

		// Enable blending for transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		// Create vertex buffer
		auto quad = std::make_shared<VertexBufferObject<Vertex2DTex>>(vertices,
		                                                              indices);
		auto circle = std::make_shared<VertexBufferObject<Vertex2DTex>>(
		    circle_vertices, indices);

		// Create shader program
		const auto paths = {"resources/shaders/pacman.vert"s,
		                    "resources/shaders/pacman.frag"s};
		m_shader_program = std::make_unique<ShaderProgram>(paths);
		m_shader_program->use();

		// Load shared sprite sheet
		m_spritesheet = std::make_shared<Texture>(
		    "resources/textures/pacman.png", GL_TEXTURE_RECTANGLE);

		// Create animations
		auto pacman_animations = create_pacman_animations();
		auto ghost_animations  = create_ghost_animations();

		// Load level file from path
		std::ifstream file(path);
		assert(file.good());
		assert(file.is_open());

		// Get level size
		file >> m_width;
		file.ignore();
		file >> m_height;

		std::vector<EntityType> grid;
		grid.resize(m_width * m_height);

		// Load level file into grid
		for (auto &cell : grid) {
			int x;
			file >> x;
			cell = static_cast<EntityType>(x);
		}

		// Spawn NUM_GHOSTS ghosts in random positions in the level
		for (size_t i = 0; i < NUM_GHOSTS; i++) {
			while (true) {
				auto pos = rnd() * grid.size();
				if (grid[pos] == EntityType::Tunnel) {
					grid[pos] = EntityType::Ghost;
					break;
				}
			}
		}

		m_total_pellets = 0;

		SpriteSheetAnimation default_pacman_animation(
		    pacman_animations->at(Direction::Right));
		SpriteSheetAnimation default_ghost_animation(
		    ghost_animations->at(Direction::Right));

		// Spawn in pacman, walls and pellets based on grid
		for (size_t i = 0; i < grid.size(); ++i) {
			auto pos = indexToCoord(i);
			switch (grid[i]) {
				case EntityType::Wall:
					m_entities.emplace_back(Wall(pos, quad));
					break;
				case EntityType::Tunnel:
					m_total_pellets++;
					m_entities.emplace_back(Pellet(pos, circle));
					break;
				case EntityType::Pacman:
					m_entities.emplace_back(Pacman(pos, quad,
					           std::make_unique<AnimatedSpriteSheet>(
					               milliseconds(50), m_spritesheet),
					           std::move(pacman_animations)));
					break;
				case EntityType::Ghost:
					m_entities.emplace_back(Ghost(pos, quad,
					          std::make_unique<AnimatedSpriteSheet>(
					              milliseconds(100), m_spritesheet),
					          ghost_animations));
					break;
			}
		}

		// Set default uniforms
		auto view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
		                        glm::vec3(0.0f, 0.0f, 0.0f),
		                        glm::vec3(0.0f, 1.0f, 0.0));
		auto projection =
		    glm::ortho(0.0f, (float)m_width, 0.0f, (float)m_height);
		auto sprite_sheet_location = 0u;

		m_spritesheet->bindToSlot(sprite_sheet_location);
		m_shader_program->setUniform("u_view", view);
		m_shader_program->setUniform("u_projection", projection);
		m_shader_program->setUniform("u_sprite_sheet", sprite_sheet_location);
	}

	/**
	 * Draw the entities of the level grid
	 */
	void draw() {
		auto &program = m_shader_program;
		auto  draw = [&program](const auto &entity) { entity.draw(*program); };
		for (const auto &entity : m_entities) {
			std::visit(draw, entity);
		}
	}

	/**
	 * Drive the simulation forward
	 */
	void update(Duration dt) {
		// Pass inputs to entities that care
		if (!m_input_queue->empty()) {
			for (auto &entity : m_entities) {
				if (std::holds_alternative<Pacman>(entity)) {
					auto &pacman = std::get<Pacman>(entity);
					auto  key    = m_input_queue->back();
					pacman.onInput(key, m_entities);
				}
			}
		}

		auto &entities = m_entities;

		// Update all entities
		for (auto &entity : m_entities) {
			//			std::visit(
			//			    [dt, &entities](auto &entity) {
			//				    using T = std::decay_t<decltype(entity)>();
			//
			//				    if constexpr (std::is_same_v<T, Pacman>) {
			//					    entity.update(dt, entities);
			//					    assert(false);
			//				    } else if constexpr (std::is_same_v<T, Ghost>) {
			//					    entity.update(dt, entities);
			//				    }
			//			    },
			//			    entity);
			if (std::holds_alternative<Pacman>(entity)) {
				auto &pacman = std::get<Pacman>(entity);
				pacman.update(dt, entities);
			} else if (std::holds_alternative<Ghost>(entity)) {
				auto &ghost = std::get<Ghost>(entity);
				ghost.update(dt, entities);
			}
		}
	}

	glm::vec2 indexToCoord(int index) {
		auto x = index % m_width;
		auto y = m_height - index / m_width;

		return glm::vec2(x, y);
	}

	int coordToIndex(std::pair<int, int> coord) {
		auto [x, y] = coord;

		int iy = y * m_width;
		int ix = x;

		return ix + iy;
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
	 * Set the dimensions and aspect ratio of the orthographic camera
	 *
	 * FIXME: Window should always show the entire grid
	 *
	 * @param dim Window/Framebuffer dimensions
	 */
	void setDimensions(const std::pair<int, int> dim) {
		auto [w, h]     = dim;
		auto aspect     = (float)w / (float)h;
		auto projection = glm::ortho(0, m_width, 0, m_height);
		m_shader_program->setUniform("u_projection", projection);
	}

	/**
	 * Is the game over?
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

	std::vector<std::variant<Wall, Pellet, Ghost, Pacman>> m_entities;
	std::unique_ptr<ShaderProgram>                         m_shader_program;
	std::shared_ptr<Texture>                               m_spritesheet;
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
	auto prev = steady_clock::now();

	// Main game loop
	while (!window.pollEvents()) {
		// Game over, but waiting to close
		if (!level.gameOver()) {
			const auto     now = steady_clock::now();
			const Duration dt  = (now - prev);
			prev               = now;

			level.update(dt);
		}

		level.draw();

		window.swapBuffers();
	}

	return EXIT_SUCCESS;
}
