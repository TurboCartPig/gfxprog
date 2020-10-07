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

const size_t NUM_GHOSTS = 4;

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
	SpriteSheetAnimation up_anim;
	up_anim.pushKeyFrame(glm::ivec4(0, 72, 72, 144));
	up_anim.pushKeyFrame(glm::ivec4(72, 72, 144, 144));
	up_anim.pushKeyFrame(glm::ivec4(144, 72, 216, 144));
	up_anim.pushKeyFrame(glm::ivec4(216, 72, 288, 144));
	up_anim.pushKeyFrame(glm::ivec4(144, 72, 216, 144));
	up_anim.pushKeyFrame(glm::ivec4(72, 72, 144, 144));

	SpriteSheetAnimation down_anim;
	down_anim.pushKeyFrame(glm::ivec4(0, 0, 72, 72));
	down_anim.pushKeyFrame(glm::ivec4(72, 0, 144, 72));
	down_anim.pushKeyFrame(glm::ivec4(144, 0, 216, 72));
	down_anim.pushKeyFrame(glm::ivec4(216, 0, 288, 72));
	down_anim.pushKeyFrame(glm::ivec4(144, 0, 216, 72));
	down_anim.pushKeyFrame(glm::ivec4(72, 0, 144, 72));

	SpriteSheetAnimation left_anim;
	left_anim.pushKeyFrame(glm::ivec4(0, 216, 72, 288));
	left_anim.pushKeyFrame(glm::ivec4(72, 216, 144, 288));
	left_anim.pushKeyFrame(glm::ivec4(144, 216, 216, 288));
	left_anim.pushKeyFrame(glm::ivec4(216, 216, 288, 288));
	left_anim.pushKeyFrame(glm::ivec4(144, 216, 216, 288));
	left_anim.pushKeyFrame(glm::ivec4(72, 216, 144, 288));

	SpriteSheetAnimation right_anim;
	right_anim.pushKeyFrame(glm::ivec4(0, 144, 72, 216));
	right_anim.pushKeyFrame(glm::ivec4(72, 144, 144, 216));
	right_anim.pushKeyFrame(glm::ivec4(144, 144, 216, 216));
	right_anim.pushKeyFrame(glm::ivec4(216, 144, 288, 216));
	right_anim.pushKeyFrame(glm::ivec4(144, 144, 216, 216));
	right_anim.pushKeyFrame(glm::ivec4(72, 144, 144, 216));

	// Put animations in a map with pacman's movement directions as the keys
	auto animations =
	    std::make_unique<std::unordered_map<Direction, SpriteSheetAnimation>>();
	animations->insert(std::make_pair(Direction::Up, up_anim));
	animations->insert(std::make_pair(Direction::Down, down_anim));
	animations->insert(std::make_pair(Direction::Right, right_anim));
	animations->insert(std::make_pair(Direction::Left, left_anim));

	return animations;
}

std::shared_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
create_ghost_animations() {
	// Define animations
	SpriteSheetAnimation up_anim;
	up_anim.pushKeyFrame(glm::ivec4(288, 72, 360, 144));
	up_anim.pushKeyFrame(glm::ivec4(360, 72, 432, 144));

	SpriteSheetAnimation down_anim;
	down_anim.pushKeyFrame(glm::ivec4(288, 0, 360, 72));
	down_anim.pushKeyFrame(glm::ivec4(360, 0, 432, 72));

	SpriteSheetAnimation right_anim;
	right_anim.pushKeyFrame(glm::ivec4(288, 144, 360, 216));
	right_anim.pushKeyFrame(glm::ivec4(360, 144, 432, 216));

	SpriteSheetAnimation left_anim;
	left_anim.pushKeyFrame(glm::ivec4(288, 216, 360, 288));
	left_anim.pushKeyFrame(glm::ivec4(360, 216, 432, 288));

	// Store animations in a map
	auto animations =
	    std::make_shared<std::unordered_map<Direction, SpriteSheetAnimation>>();
	animations->insert(std::make_pair(Direction::Up, up_anim));
	animations->insert(std::make_pair(Direction::Down, down_anim));
	animations->insert(std::make_pair(Direction::Right, right_anim));
	animations->insert(std::make_pair(Direction::Left, left_anim));

	return animations;
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

		SpriteSheetAnimation default_pacman_animation(
		    pacman_animations->at(Direction::Right));
		SpriteSheetAnimation default_ghost_animation(
		    ghost_animations->at(Direction::Right));

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

		// Spawn in pacman, walls and pellets based on grid
		for (size_t i = 0; i < grid.size(); ++i) {
			auto x   = i % m_width;
			auto y   = m_height - i / m_width - 1;
			auto pos = glm::vec2(x, y);
			switch (grid[i]) {
				case EntityType::Wall:
					m_entities.emplace_back(Wall(pos));
					break;
				case EntityType::Tunnel:
					m_total_pellets++;
					m_entities.emplace_back(Pellet(pos));
					break;
				case EntityType::Pacman:
					m_entities.emplace_back(
					    Pacman(pos,
					           std::make_unique<AnimatedSpriteSheet>(
					               milliseconds(50), m_spritesheet),
					           std::move(pacman_animations)));
					break;
				case EntityType::Ghost: // This is a bit hacky
				                        // Add in pellet first
					m_total_pellets++;
					m_entities.emplace_back(Pellet(pos));
					// Then add ghost in same position
					m_entities.emplace_back(
					    Ghost(pos,
					          std::make_unique<AnimatedSpriteSheet>(
					              milliseconds(100), m_spritesheet),
					          ghost_animations));
					break;
				default: continue;
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

		m_vbo = std::make_unique<VertexBuffer<Vertex2DTexRgbav>>(
		    m_entities.size(), true, GL_STREAM_DRAW);
	}

	/**
	 * Draw the entities of the level grid
	 */
	void draw() {
		std::vector<Vertex2DTexRgbav> ver;
		std::vector<uint32_t>         ind;

		// Sort entities to be backmost first
		// FIXME: Change on change instead of eagerly
		std::sort(m_entities.begin(), m_entities.end(),
		          [](const auto &a, const auto &b) {
			          auto get = [](const auto &e) { return e.getZPosition(); };
			          auto az  = std::visit(get, a);
			          auto bz  = std::visit(get, b);
			          return az < bz;
		          });

		// Generate vertex contents
		auto draw = [](const auto &entity) { return entity.getAttributes(); };
		for (const auto &entity : m_entities) {
			auto      attrib  = std::visit(draw, entity);
			uint32_t  variant = attrib.rgba.index();
			glm::vec4 rgba;

			if (variant == 0) {
				rgba = std::get<glm::vec4>(attrib.rgba);
			} else {
				rgba = std::get<glm::ivec4>(attrib.rgba);
			}

			auto offset = ver.size();
			if (attrib.quad) { // Add quad
				ver.push_back(
				    {attrib.scale * glm::vec2(0.0f, 0.0f) + attrib.position,
				     glm::vec2(1.0f, 1.0f), rgba, variant});
				ver.push_back(
				    {attrib.scale * glm::vec2(0.0f, 1.0f) + attrib.position,
				     glm::vec2(1.0f, 0.0f), rgba, variant});
				ver.push_back(
				    {attrib.scale * glm::vec2(1.0f, 0.0f) + attrib.position,
				     glm::vec2(0.0f, 1.0f), rgba, variant});
				ver.push_back(
				    {attrib.scale * glm::vec2(1.0f, 1.0f) + attrib.position,
				     glm::vec2(0.0f, 0.0f), rgba, variant});
				ind.push_back(offset + 0);
				ind.push_back(offset + 1);
				ind.push_back(offset + 2);
				ind.push_back(offset + 1);
				ind.push_back(offset + 2);
				ind.push_back(offset + 3);
			} else { // Add diamond
				// FIXME: Add circles instead of diamonds
				ver.push_back(
				    {attrib.scale * glm::vec2(0.25f, 0.5f) + attrib.position,
				     glm::vec2(1.0f, 1.0f), rgba, variant});
				ver.push_back(
				    {attrib.scale * glm::vec2(0.5f, 0.75f) + attrib.position,
				     glm::vec2(0.0f, 1.0f), rgba, variant});
				ver.push_back(
				    {attrib.scale * glm::vec2(0.5f, 0.25f) + attrib.position,
				     glm::vec2(0.0f, 0.0f), rgba, variant});
				ver.push_back(
				    {attrib.scale * glm::vec2(0.75f, 0.5f) + attrib.position,
				     glm::vec2(1.0f, 0.0f), rgba, variant});
				ind.push_back(offset + 0);
				ind.push_back(offset + 1);
				ind.push_back(offset + 2);
				ind.push_back(offset + 1);
				ind.push_back(offset + 2);
				ind.push_back(offset + 3);
			}
		}

		m_vbo->uploadWhole(ver, ind);
		m_vbo->draw();
	}

	/**
	 * Drive the simulation forward
	 */
	void update(Duration dt) {
		// Update all entities
		for (auto &entity : m_entities) {
			if (std::holds_alternative<Pacman>(entity)) { // Update pacman
				auto &pacman = std::get<Pacman>(entity);

				// Pass input if there is any
				if (!m_input_queue->empty()) {
					auto key = m_input_queue->back();
					pacman.onInput(key, m_entities);
				}

				// Update
				pacman.update(dt, m_entities);

				// Check if the game should end
				if (!pacman.active()) {
					endGame(pacman.getPelletsEaten());
				} else if (pacman.getPelletsEaten() >= m_total_pellets) {
					winGame();
				}
			} else if (std::holds_alternative<Ghost>(entity)) { // Update ghosts
				auto &ghost = std::get<Ghost>(entity);
				ghost.update(dt, m_entities);
			}
		}

		// Delete disabled entities
		for (auto i = m_entities.size() - 1; i != 0; i--) {
			auto active =
			    std::visit([](const auto &entity) { return entity.active(); },
			               m_entities[i]);
			if (!active) {
				std::swap(m_entities[i], m_entities[m_entities.size() - 1]);
				m_entities.pop_back();
			}
		}
	}

	/**
	 * End the game.
	 */
	void endGame(int pellets_eaten) {
		std::cout << "Game Over" << std::endl;
		std::cout << pellets_eaten << " pellets eaten out of "
		          << m_total_pellets << std::endl;
		m_isGameOver = true;
	}

	/**
	 * Win the game.
	 */
	void winGame() {
		std::cout << "All pellets eaten, Game Won!" << std::endl;
		m_isGameOver = true;
	}

	/**
	 * Set the dimensions and aspect ratio of the orthographic camera
	 * @param dim Window/Framebuffer dimensions
	 */
	void setDimensions(const std::pair<int, int> dim) {
		auto [w, h]  = dim;
		float aspect = (float)w / (float)h;
		auto  projection =
		    glm::ortho(0.0f, aspect * m_width, 0.0f, (float)m_height);
		m_shader_program->setUniform("u_projection", projection);
	}

	/**
	 * Is the game over?
	 * @return true if the game is over
	 */
	[[nodiscard]] bool isGameOver() const { return m_isGameOver; }

  private:
	bool m_isGameOver    = false;
	int  m_total_pellets = 0;

	int m_width  = 0;
	int m_height = 0;

	InputQueue m_input_queue;

	Entities                                        m_entities;
	std::unique_ptr<ShaderProgram>                  m_shader_program;
	std::shared_ptr<Texture>                        m_spritesheet;
	std::unique_ptr<VertexBuffer<Vertex2DTexRgbav>> m_vbo;
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
		if (!level.isGameOver()) {
			const auto     now = steady_clock::now();
			const Duration dt  = (now - prev);
			prev               = now;

			level.update(dt);
		}

		level.draw();
		level.setDimensions(window.dimensions());
		window.swapBuffers();
	}

	return EXIT_SUCCESS;
}
