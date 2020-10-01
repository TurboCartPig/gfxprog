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

/**
 * Abstract base class
 */
class SpriteEntity {
  public:
	explicit SpriteEntity(glm::vec2                                        pos,
	                      std::shared_ptr<VertexBufferObject<Vertex2DTex>> vbo)
	    : m_position(pos), m_vbo(std::move(vbo)) {}

	virtual void draw(ShaderProgram &program) const { m_vbo->draw(); }

	glm::vec2 getPosition() const { return m_position; }

  protected:
	std::shared_ptr<VertexBufferObject<Vertex2DTex>> m_vbo;
	glm::vec2                                        m_position;
};

class Pacman : public SpriteEntity {
  public:
	Pacman(glm::vec2 pos, std::shared_ptr<VertexBufferObject<Vertex2DTex>> vbo,
	       std::unique_ptr<AnimatedSpriteSheet> spritesheet,
	       std::unique_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
	           animations)
	    : SpriteEntity(pos, std::move(vbo)), m_direction(Direction::Right),
	      m_spritesheet(std::move(spritesheet)),
	      m_animations(std::move(animations)) {
		m_spritesheet->playAnimation(m_animations->at(Direction::Right));
	}

	void onInput(
	    InputCode                                      key,
	    const std::vector<std::variant<class Wall, class Pellet, class Ghost,
	                                   class Pacman>> &entities);

	void
	update(Duration                                       dt,
	       const std::vector<std::variant<class Wall, class Pellet, class Ghost,
	                                      class Pacman>> &entities);

	void draw(ShaderProgram &program) const override;

  private:
	int                                  m_pellets_eaten = 0;
	Direction                            m_direction;
	std::unique_ptr<AnimatedSpriteSheet> m_spritesheet;
	std::unique_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
	    m_animations;
};

class Ghost : public SpriteEntity {
  public:
	Ghost(glm::vec2 pos, std::shared_ptr<VertexBufferObject<Vertex2DTex>> vbo,
	      std::unique_ptr<AnimatedSpriteSheet> spritesheet,
	      std::shared_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
	          animations)
	    : SpriteEntity(pos, std::move(vbo)),
	      m_spritesheet(std::move(spritesheet)),
	      m_animations(std::move(animations)) {
		m_spritesheet->playAnimation(m_animations->at(Direction::Right));
	}

	void
	update(Duration                                       dt,
	       const std::vector<std::variant<class Wall, class Pellet, class Ghost,
	                                      class Pacman>> &entities) {
		m_spritesheet->update(dt);
	}

	void draw(ShaderProgram &program) const override {
		auto transform =
		    glm::translate(glm::mat4(1.0f), glm::vec3(m_position, 0.0f));
		program.setUniform("u_transform", transform);
		program.setUniform("u_color", glm::vec4(0.0f));
		program.setUniform("u_key_frame", m_spritesheet->getUniform());
		SpriteEntity::draw(program);
	}

  private:
	std::unique_ptr<AnimatedSpriteSheet> m_spritesheet;
	std::shared_ptr<std::unordered_map<Direction, SpriteSheetAnimation>>
	    m_animations;
};

class Pellet : public SpriteEntity {
  public:
	Pellet(glm::vec2 pos, std::shared_ptr<VertexBufferObject<Vertex2DTex>> vbo)
	    : SpriteEntity(pos, std::move(vbo)) {}

	void draw(ShaderProgram &program) const override {
		if (!active)
			return;

		auto transform = glm::mat4(1.0f);
		transform      = glm::translate(
            transform, glm::vec3(m_position + glm::vec2(0.5f, 0.5f), 0.0f));
		transform = glm::scale(transform, glm::vec3(0.25f, 0.25f, 0.0f));
		program.setUniform("u_transform", transform);
		program.setUniform("u_color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		SpriteEntity::draw(program);
	}

	void pickup() const { active = false; }

  private:
	mutable bool active = true;
};

class Wall : public SpriteEntity {
  public:
	explicit Wall(glm::vec2                                        pos,
	              std::shared_ptr<VertexBufferObject<Vertex2DTex>> vbo)
	    : SpriteEntity(pos, std::move(vbo)) {}

	void draw(ShaderProgram &program) const override {
		auto transform =
		    glm::translate(glm::mat4(1.0f), glm::vec3(m_position, 0.0f));
		program.setUniform("u_transform", transform);
		program.setUniform("u_color", m_color);
		SpriteEntity::draw(program);
	}

  private:
	glm::vec4 m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
};
