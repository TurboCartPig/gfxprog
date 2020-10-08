#pragma once

#include <chrono>
#include <glove/lib.h>
#include <memory>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

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
	glm::vec2 position; ///< World space position
	std::variant<glm::vec4, glm::ivec4>
	      rgba;  ///< If color is variant ivec4 it is a keyframe not a color.
	float scale; ///< Uniform scale
	bool  quad;  ///< Is textured quad. If no we assume it's a circle
};

static Attributes make_attributes(glm::vec2 position, glm::vec4 rgba,
                                  float scale = 1.0f, bool quad = true) {
	return Attributes{position, rgba, scale, quad};
}

static Attributes make_attributes(glm::vec2 position, glm::ivec4 keyframe,
                                  float scale = 1.0f, bool quad = true) {
	return Attributes{position, keyframe, scale, quad};
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
	 * Get the z position of the entity in "3D" space. Used for render order.
	 * @return
	 */
	[[nodiscard]] virtual float getZPosition() const = 0;

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
	void deactivate() { m_active = false; }

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
	                                       animations,
	       std::pair<glm::vec2, glm::vec2> bounds)
	    : SpriteEntity(pos), m_direction(Direction::Right),
	      m_spritesheet(std::move(spritesheet)),
	      m_animations(std::move(animations)), m_bounds(std::move(bounds)) {
		m_spritesheet->playAnimation(m_animations->at(Direction::Right));
	}

	void onInput(InputCode key, const Entities &entities);

	void update(Duration dt, Entities &entities);

	[[nodiscard]] Attributes getAttributes() const override {
		return make_attributes(m_position, m_spritesheet->getUniform());
	}

	[[nodiscard]] float getZPosition() const override { return 2.0f; }

	[[nodiscard]] int getPelletsEaten() const { return m_pellets_eaten; }

  private:
	int                                  m_pellets_eaten = 0;
	Direction                            m_direction;
	std::pair<glm::vec2, glm::vec2>      m_bounds;
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
	                                      animations,
	      std::pair<glm::vec2, glm::vec2> bounds)
	    : SpriteEntity(pos), m_spritesheet(std::move(spritesheet)),
	      m_animations(std::move(animations)), m_direction(Direction::Right),
	      m_bounds(std::move(bounds)) {
		m_spritesheet->playAnimation(m_animations->at(m_direction));
	}

	void update(Duration dt, const Entities &entities);

	[[nodiscard]] Attributes getAttributes() const override {
		return make_attributes(m_position, m_spritesheet->getUniform());
	}

	[[nodiscard]] float getZPosition() const override { return 3.0f; }

  private:
	Direction                            m_direction;
	std::pair<glm::vec2, glm::vec2>      m_bounds;
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
		return make_attributes(m_position, glm::vec4(1.0f), 1.0f, false);
	}

	[[nodiscard]] float getZPosition() const override { return 1.0f; }
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

	[[nodiscard]] float getZPosition() const override { return 0.0f; }

  private:
	glm::vec4 m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
};
