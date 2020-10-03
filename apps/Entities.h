#pragma once

#include <chrono>
#include <functional>
#include <glove/lib.h>
#include <memory>
#include <utility>
#include <variant>

using Duration = std::chrono::duration<float>;

/**
 * Direction of movement.
 */
enum class Direction {
	Left,
	Right,
	Up,
	Down,
};

struct Attributes {
	// World space position
	glm::vec2 position;
	// If color is variant ivec4 it is a keyframe not a color.
	std::variant<glm::vec4, glm::ivec4> rgba;
	// Uniform scale
	float scale;
};

static Attributes make_attributes(glm::vec2 position, glm::vec4 rgba,
                                  float scale = 1.0f) {
	return Attributes{position, rgba, scale};
}

static Attributes make_attributes(glm::vec2 position, glm::ivec4 keyframe,
                                  float scale = 1.0f) {
	return Attributes{position, keyframe, scale};
}

/**
 * Abstract base class for shared functionality for all game objects.
 */
class SpriteEntity {
  public:
	explicit SpriteEntity(glm::vec2 pos) : m_position(pos) {}

	virtual ~SpriteEntity() = default;

	virtual Attributes getAttributes() const = 0;

	glm::vec2 getPosition() const { return m_position; }

	bool active() const { return m_active; }

	void deactivate() {
		m_active = false;
		std::cout << "Deactivated\n";
	}

  protected:
	bool      m_active = true;
	glm::vec2 m_position;
};

/**
 * Pacman.
 */
class Pacman : public SpriteEntity {
  public:
	Pacman(glm::vec2 pos, std::unique_ptr<AnimatedSpriteSheet> spritesheet,
	       std::unique_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
	           animations)
	    : SpriteEntity(pos), m_direction(Direction::Right),
	      m_spritesheet(std::move(spritesheet)),
	      m_animations(std::move(animations)) {
		m_spritesheet->playAnimation(m_animations->at(Direction::Right));
	}

	void onInput(
	    InputCode                                      key,
	    const std::vector<std::variant<class Wall, class Pellet, class Ghost,
	                                   class Pacman>> &entities);

	void update(Duration                                 dt,
	            std::vector<std::variant<class Wall, class Pellet, class Ghost,
	                                     class Pacman>> &entities);

	Attributes getAttributes() const override {
		return make_attributes(m_position, m_spritesheet->getUniform());
	}

  private:
	int                                  m_pellets_eaten = 0;
	Direction                            m_direction;
	std::unique_ptr<AnimatedSpriteSheet> m_spritesheet;
	std::unique_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
	    m_animations;
};

/**
 * Ghost.
 */
class Ghost : public SpriteEntity {
  public:
	Ghost(glm::vec2 pos, std::unique_ptr<AnimatedSpriteSheet> spritesheet,
	      std::shared_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
	          animations)
	    : SpriteEntity(pos), m_spritesheet(std::move(spritesheet)),
	      m_animations(std::move(animations)) {
		m_spritesheet->playAnimation(m_animations->at(Direction::Right));
	}

	void
	update(Duration                                       dt,
	       const std::vector<std::variant<class Wall, class Pellet, class Ghost,
	                                      class Pacman>> &entities);

	Attributes getAttributes() const override {
		return make_attributes(m_position, m_spritesheet->getUniform());
	}

  private:
	std::unique_ptr<AnimatedSpriteSheet> m_spritesheet;
	std::shared_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
	    m_animations;
};

/**
 * Pellet.
 */
class Pellet : public SpriteEntity {
  public:
	Pellet(glm::vec2 pos) : SpriteEntity(pos) {}

	Attributes getAttributes() const override {
		return make_attributes(m_position, glm::vec4(1.0f), 0.25f);
	}
};

/**
 * Wall.
 */
class Wall : public SpriteEntity {
  public:
	Wall(glm::vec2 pos) : SpriteEntity(pos) {}

	Attributes getAttributes() const override {
		return make_attributes(m_position, m_color);
	}

  private:
	glm::vec4 m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
};
