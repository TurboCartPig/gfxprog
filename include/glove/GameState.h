#pragma once

#include <glove/Window.h>
#include <memory>
#include <variant>
#include <vector>

/**
 * @brief No state transition should occur.
 */
struct None {};

/**
 * @brief Pop the current state off the stack.
 * If there are other states on the stack, the top one will be the next active
 * state. If there are no other states on the stack, the application will close.
 */
struct Pop {};

/**
 * @brief Push a new state to the top of the stack.
 * This new state will be the next active state.
 */
struct Push {
	std::unique_ptr<class IGameState> state;
};

/**
 * @brief Transition laterally to a new state. Think of it as a pop and push in
 * one operation.
 */
struct Transition {
	std::unique_ptr<class IGameState> state;
};

/**
 * @brief Should the current state transition into a new state? If so how?
 */
using StateTransition = std::variant<None, Push, Pop, Transition>;

/**
 * @brief Interface for a game state object.
 */
class IGameState {
  public:
	IGameState() = default;

	IGameState(const IGameState &other) = delete;

	IGameState(const IGameState &&other) = delete;

	const IGameState &operator=(const IGameState &other) = delete;

	const IGameState &&operator=(const IGameState &&other) = delete;

	virtual ~IGameState() = default;

	virtual void initialize() = 0;

	/**
	 * @brief Pass player input to the game state.
	 * @param code Key code
	 * @return Should the game transition states as a result of the input? If so
	 * how?
	 */
	[[nodiscard]] virtual StateTransition input(Input input) = 0;

	/**
	 * @brief Drive the game state forward.
	 * @param dt Delta time.
	 * @return Should the game transition states as a result of the input? If so
	 * how?
	 */
	[[nodiscard]] virtual StateTransition update(float dt) = 0;

	/**
	 * @brief Render the game as of this game state.
	 */
	virtual void render() = 0;
};

/**
 * @brief Interface for an overlay on top of the game state stack.
 *
 * TODO: Implement overlays
 * - For example an imgui overlay for debugging and development.
 */
class IOverlay {
  public:
	virtual void attach()              = 0;
	virtual void detach()              = 0;
	virtual void input(InputCode code) = 0;
	virtual void update(float dt)      = 0;
	virtual void render()              = 0;
};

/**
 * @brief Core engine driver.
 */
class Core {
  public:
	/**
	 * @brief Create a new core with an initial state.
	 * @param initial_state The initial state of the game.
	 */
	Core(std::unique_ptr<IGameState> initial_state);

	/**
	 * @brief Run the game.
	 * Update states and pass them input. Push and pop states based on state
	 * transitions.
	 */
	void run();

  private:
	std::unique_ptr<Window>                  m_window;
	std::vector<std::unique_ptr<IGameState>> m_gamestates;
	//	std::unordered_map<std::string, std::unique_ptr<IOverlay>> m_overlays;
};