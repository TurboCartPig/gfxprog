#include "Level.h"

#include <cassert>
#include <fstream>

Level::Level(const std::string &path) {
	// Load level file from path
	std::ifstream file(path);
	assert(file.good());
	assert(file.is_open());

	// Get level size
	file >> m_width;
	file.ignore();
	file >> m_height;

	m_grid.resize(m_width * m_height);

	// Load level file into grid
	for (auto &cell : m_grid) {
		int x;
		file >> x;
		cell = static_cast<EntityType>(x);
	}
}
