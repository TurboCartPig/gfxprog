#pragma once

#include <chrono>
#include <functional>
#include <glove/lib.h>
#include <memory>
#include <utility>
#include <variant>

using Duration = std::chrono::duration<float>;
using Entities = std::vector<
    std::variant<class Wall, class Pellet, class Ghost, class Pacman>>;

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

	/**
	 * Get info used to generate vertex attributes.
	 * @return Intermediate vertex attribute information
	 */
	[[nodiscard]] virtual Attributes getAttributes() const = 0;

	/**
	 * Get the position of the entity in world space coordinates.
	 * @return Position in world space coordinates.
	 */
	[[nodiscard]] glm::vec2 getPosition() const { return m_position; }

	/**
	 * Is the entity active?
	 * If an entity is no longer active, it will be deleted.
	 * @return Is active?
	 */
	[[nodiscard]] bool active() const { return m_active; }

	/**
	 * Deactivate the entity.
	 * Entity will be deleted.
	 */
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

	void onInput(InputCode key, const Entities &entities);

	void update(Duration dt, Entities &entities);

	[[nodiscard]] Attributes getAttributes() const override {
		return make_attributes(m_position, m_spritesheet->getUniform());
	}

	[[nodiscard]] int getPelletsEaten() const { return m_pellets_eaten; }

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
	      m_animations(std::move(animations)), m_direction(Direction::Right) {
		m_spritesheet->playAnimation(m_animations->at(m_direction));
	}

	void update(Duration dt, const Entities &entities);

	[[nodiscard]] Attributes getAttributes() const override {
		return make_attributes(m_position, m_spritesheet->getUniform());
	}

  private:
	Direction                            m_direction;
	std::unique_ptr<AnimatedSpriteSheet> m_spritesheet;
	std::shared_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
	    m_animations;
};

/**
 * Pellet.
 */
class Pellet : public SpriteEntity {
  public:
	explicit Pellet(glm::vec2 pos) : SpriteEntity(pos) {}

	[[nodiscard]] Attributes getAttributes() const override {
		return make_attributes(m_position, glm::vec4(1.0f), 0.25f);
	}
};

/**
 * Wall.
 */
class Wall : public SpriteEntity {
  public:
	explicit Wall(glm::vec2 pos) : SpriteEntity(pos) {}

	[[nodiscard]] Attributes getAttributes() const override {
		return make_attributes(m_position, m_color);
	}

  private:
	glm::vec4 m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
};
