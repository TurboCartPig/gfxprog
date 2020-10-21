#include <cassert>
#include <fstream>
#include <glove/lib.h>

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

/**
 * Abstract level representation.
 */
class Level {
  public:
	Level(const std::string &path) {
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

  private:
	int                     m_width;
	int                     m_height;
	std::vector<EntityType> m_grid;
};

/**
 * Main game state of pacman 3d.
 */
class GameState : public IGameState {
  public:
	GameState() = default;

	[[nodiscard]] StateTransition input(InputCode code) override {
		if (code == InputCode::Escape)
			return Pop{};
		else if (code == InputCode::A)
			return Push{std::make_unique<GameState>()};

		return None{};
	}

	[[nodiscard]] StateTransition update(float dt) override { return None{}; }

	void render() override {}

  private:
	//  Pacman m_pacman;
	//	std::vector<Ghost> m_ghosts;
};

int main() {
	auto core = Core(std::make_unique<GameState>());
	core.run();

	return 0;
}