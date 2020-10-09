#include "Entities.h"

/**
 * Convert movement direction to delta position.
 * @param direction Direction of movement.
 * @return Delta position.
 */
static glm::vec2 direction_to_delta(Direction direction) {
	glm::vec2 delta;
	switch (direction) {
		case Direction::Up: delta = glm::vec2(0.0f, 1.0f); break;
		case Direction::Down: delta = glm::vec2(0.0f, -1.0f); break;
		case Direction::Right: delta = glm::vec2(1.0f, 0.0f); break;
		case Direction::Left: delta = glm::vec2(-1.0f, 0.0f); break;
	}

	return delta;
}

/**
 * Check if the "position" is within the level bounds.
 * @param position Position to check
 * @param bounds Level bounds.
 * @return Is "position" within the level bounds.
 */
static bool checkBounds(glm::vec2                       position,
                        std::pair<glm::vec2, glm::vec2> bounds) {
	auto [min, max] = bounds;
	return position.x >= min.x && position.y >= min.y && position.x <= max.x &&
	       position.y <= max.y;
}

/**
 * Check if "position" collides with position of "T". I.e. is closer than
 * "dist". The index, into "entities" of the colliding entity is returned via
 * "index".
 * @tparam T Type of entity to check collision with.
 * @param position Position of the entity to check against all the "T"s in
 * "entities".
 * @param entities All the entities to check against.
 * @param dist Distance that if less than, gives a collision.
 * @param index Index of the entity collided with. Not changed if no collision.
 * @return Does the entities overlap?
 */
template <typename T>
static bool checkCollision(glm::vec2 position, const Entities &entities,
                           float dist = 0.95f, size_t *index = nullptr) {
	for (size_t i = 0; i < entities.size(); i++) {
		if (std::holds_alternative<T>(entities[i])) {
			const auto &t = std::get<T>(entities[i]);
			if (glm::length(t.getPosition() - position) <= dist) {
				if (index)
					*index = i;
				return true;
			}
		}
	}

	return false;
}

void Pacman::onInput(InputCode key, const Entities &entities) {
	// Input -> Direction
	Direction new_direction;
	switch (key) {
		case InputCode::W: new_direction = Direction::Up; break;
		case InputCode::S: new_direction = Direction::Down; break;
		case InputCode::A: new_direction = Direction::Left; break;
		case InputCode::D: new_direction = Direction::Right; break;
		default: return;
	}

	// Would pacman collide with a wall if he headed in the new direction?
	auto delta     = direction_to_delta(new_direction);
	bool collision = checkCollision<Wall>(m_position + delta, entities, 0.80f);

	// Only change direction if it is a new direction and pacman would not
	// collide in the immediate future
	if (!collision && new_direction != m_direction) {
		m_direction = new_direction;

		// Change pacman's animation
		m_spritesheet->playAnimation(m_animations->at(m_direction));
	}
}

void Pacman::update(Duration dt, Entities &entities) {
	// Find new position if pacman moved
	auto delta    = direction_to_delta(m_direction);
	auto position = m_position + dt.count() * delta * 2.0f;

	// Is pacman within bounds?
	bool in_bounds = checkBounds(position, m_bounds);
	// Is pacman colliding with a wall?
	bool collision = checkCollision<Wall>(position, entities);

	// Should pacman eat a pellet? If so what is the index of that entity?
	size_t index = 0;
	bool   pellet_eaten =
	    checkCollision<Pellet>(position, entities, 0.20f, &index);

	// Disable / delete the eaten pellet
	if (pellet_eaten) {
		std::get<Pellet>(entities[index]).deactivate();
		m_pellets_eaten++;
	}

	// Is pacman colliding with a ghost?
	bool hit_ghost = checkCollision<Ghost>(position, entities, 0.35f);
	// If he is, deactivate / delete pacman
	if (hit_ghost)
		this->deactivate();

	if (!collision && in_bounds) {
		// Remember to start playing animation again if newly out of collision
		m_spritesheet->play();
		m_position = position;
	} else {
		m_spritesheet->play(false);
		// Snap pacman to grid to avoid odd collisions in the middle of tunnels
		if (m_direction == Direction::Up || m_direction == Direction::Down)
			m_position.x = std::round(m_position.x);
		else
			m_position.y = std::round(m_position.y);
	}

	m_spritesheet->update(dt);
}

void Ghost::update(Duration dt, const Entities &entities) {
	// Pseudo ai
	// Currently gets stuck a lot.

	// Find new position
	auto delta    = direction_to_delta(m_direction);
	auto position = m_position + delta * dt.count() * 1.5f;

	// Is the ghost within bounds?
	bool in_bounds = checkBounds(position, m_bounds);
	// Would we collide with a wall if we move to new position?
	bool collide_with_wall = checkCollision<Wall>(position, entities);
	if (collide_with_wall || !in_bounds) {
		switch (m_direction) {
			case Direction::Up: m_direction = Direction::Right; break;
			case Direction::Right: m_direction = Direction::Down; break;
			case Direction::Down: m_direction = Direction::Left; break;
			case Direction::Left: m_direction = Direction::Up; break;
		}
		m_spritesheet->playAnimation(m_animations->at(m_direction));
	} else {
		m_position = position;
	}

	m_spritesheet->update(dt);
}
