#include "Entities.h"

template <typename T>
bool checkCollision(
    glm::vec2                                                     position,
    const std::vector<std::variant<Wall, Pellet, Ghost, Pacman>> &entities,
    float dist = 0.95f, std::function<void(const T &entity)> f = nullptr) {
	bool collision = false;

	for (const auto &entity : entities) {
		if (std::holds_alternative<T>(entity)) {
			auto &t = std::get<T>(entity);
			if (glm::length(t.getPosition() - position) <= dist) {
				collision = true;
				if (f != nullptr) {
					f(t);
				}
			}
		}
	}

	return collision;
}

void Pacman::onInput(
    InputCode                                                     key,
    const std::vector<std::variant<Wall, Pellet, Ghost, Pacman>> &entities) {
	Direction new_direction;
	switch (key) {
		case InputCode::W: new_direction = Direction::Up; break;
		case InputCode::S: new_direction = Direction::Down; break;
		case InputCode::A: new_direction = Direction::Left; break;
		case InputCode::D: new_direction = Direction::Right; break;
		default: return;
	}

	//	bool collision = checkCollision(m_position, entities);
	//	if (collision)
	//		return;

	m_direction = new_direction;

	// Change pacman's animation
	m_spritesheet->playAnimation(m_animations->at(m_direction));
}

void Pacman::update(
    Duration                                                      dt,
    const std::vector<std::variant<Wall, Pellet, Ghost, Pacman>> &entities) {
	glm::vec2 delta;

	switch (m_direction) {
		case Direction::Up: delta = glm::vec2(0.0f, 1.0f); break;
		case Direction::Down: delta = glm::vec2(0.0f, -1.0f); break;
		case Direction::Right: delta = glm::vec2(1.0f, 0.0f); break;
		case Direction::Left: delta = glm::vec2(-1.0f, 0.0f); break;
	}

	auto position = m_position + dt.count() * delta;

	bool collision    = checkCollision<Wall>(position, entities);
	bool pellet_eaten = checkCollision<Pellet>(
	    position, entities, 0.25f, [](const Pellet &p) { p.pickup(); });
	bool hit_ghost = checkCollision<Ghost>(position, entities, 0.5f);

	assert(!hit_ghost);

	if (pellet_eaten)
		m_pellets_eaten++;

	if (!collision) {
		m_spritesheet->play();
		m_position = position;
	} else {
		m_spritesheet->play(false);
		if (m_direction == Direction::Up || m_direction == Direction::Down)
			m_position.x = std::round(m_position.x);
		else
			m_position.y = std::round(m_position.y);
	}

	m_spritesheet->update(dt);
}

void Pacman::draw(ShaderProgram &program) const {
	auto transform =
	    glm::translate(glm::mat4(1.0f), glm::vec3(m_position, 0.0f));
	program.setUniform("u_transform", transform);
	program.setUniform("u_color", glm::vec4(0.0f));
	program.setUniform("u_key_frame", m_spritesheet->getUniform());
	SpriteEntity::draw(program);
}
