#include "Entities.h"

glm::vec2 direction_to_delta(Direction direction) {
	glm::vec2 delta;
	switch (direction) {
		case Direction::Up: delta = glm::vec2(0.0f, 1.0f); break;
		case Direction::Down: delta = glm::vec2(0.0f, -1.0f); break;
		case Direction::Right: delta = glm::vec2(1.0f, 0.0f); break;
		case Direction::Left: delta = glm::vec2(-1.0f, 0.0f); break;
	}

	return delta;
}

template <typename T>
static bool checkCollision(
    glm::vec2                                                     position,
    const std::vector<std::variant<Wall, Pellet, Ghost, Pacman>> &entities,
    float dist = 0.95f, size_t *index = nullptr) {
	bool collision = false;

	for (size_t i = 0; i < entities.size(); i++) {
		if (std::holds_alternative<T>(entities[i])) {
			auto &t = std::get<T>(entities[i]);
			if (glm::length(t.getPosition() - position) <= dist) {
				collision = true;
				if (index)
					*index = i;
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
    Duration                                                dt,
    std::vector<std::variant<Wall, Pellet, Ghost, Pacman>> &entities) {
	// Find new position if pacman moved
	auto delta    = direction_to_delta(m_direction);
	auto position = m_position + dt.count() * delta;

	bool collision = checkCollision<Wall>(position, entities);

	size_t index;
	bool   pellet_eaten =
	    checkCollision<Pellet>(position, entities, 0.25f, &index);
	if (pellet_eaten)
		std::get<Pellet>(entities[index]).deactivate();

	bool hit_ghost = checkCollision<Ghost>(position, entities, 0.5f);
	if (hit_ghost)
		this->deactivate();

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

void Ghost::update(
    Duration                                       dt,
    const std::vector<std::variant<class Wall, class Pellet, class Ghost,
                                   class Pacman>> &entities) {
	// Pseudo ai
	// Currently gets stuck a lot.

	auto delta    = direction_to_delta(m_direction);
	auto position = m_position + delta * dt.count();

	bool collide_with_wall = checkCollision<Wall>(position, entities);
	if (collide_with_wall) {
		switch (m_direction) {
			case Direction::Up: m_direction = Direction::Right; break;
			case Direction::Right: m_direction = Direction::Down; break;
			case Direction::Down: m_direction = Direction::Left; break;
			case Direction::Left: m_direction = Direction::Up; break;
		}
	} else {
		m_position = position;
	}

	m_spritesheet->update(dt);
}
