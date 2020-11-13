#pragma once

#include <string>
#include <vector>

/**
 * @brief The type/variant of entity in the level grid.
 */
enum class EntityType : uint32_t {
	Tunnel = 0,
	Wall   = 1,
	Pacman = 2,
	Ghost  = 3,
	Pellet = 4,
};

/**
 * @brief Abstract level representation.
 */
class Level {
  public:
	Level(const std::string &path);

	/**
	 * @brief Get the type of entity at the given x,y coordinate.
	 * @param x X coordinate / width index.
	 * @param y T coordinate / height index.
	 * @return Type of entity in the level at give coordinates.
	 */
	[[nodiscard]] auto get(int x, int y) const {
		x = std::max(0, std::min(m_width, x));
        y = std::max(0, std::min(m_height, y));
		return m_grid[y * m_width + x];
	}

	/**
	 * @brief Get the size of the level with entities as units.
	 * @return Width / Height of the level.
	 */
	[[nodiscard]] auto getSize() const noexcept {
		return std::make_pair(m_width, m_height);
	}

  private:
	int                     m_width;
	int                     m_height;
	std::vector<EntityType> m_grid;
};
