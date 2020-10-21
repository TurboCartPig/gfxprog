#pragma once

#include <glove/Window.h>
#include <memory>
#include <variant>
#include <vector>

/**
 * No state transition should occur.
 */
struct None {};

/**
 * Pop the current state off the stack.
 * If there are other states on the stack, the top one will be the next active
 * state. If there are no other states on the stack, the application will close.
 */
struct Pop {};

/**
 * Push a new state to the top of the stack.
 * This new state will be the next active state.
 */
struct Push {
	std::unique_ptr<class IGameState> state;
};

/**
 * Transition laterally to a new state. Think of it as a pop and push in one
 * operation.
 */
struct Transition {
	std::unique_ptr<class IGameState> state;
};

/**
 * Should the current state transition into a new state? If so how?
 */
using StateTransition = std::variant<None, Push, Pop, Transition>;

/**
 * Interface for a game state object.
 */
class IGameState {
  public:
	virtual ~IGameState() = default;

	/**
	 * Pass player input to the game state.
	 * @param code Key code
	 * @return Should the game transition states as a result of the input? If so
	 * how?
	 */
	[[nodiscard]] virtual StateTransition input(InputCode code) = 0;

	/**
	 * Drive the game state forward.
	 * @param dt Delta time.
	 * @return Should the game transition states as a result of the input? If so
	 * how?
	 */
	[[nodiscard]] virtual StateTransition update(float dt) = 0;

	/**
	 * Render the game as of this game state.
	 */
	virtual void render() = 0;
};

/**
 * Interface for an overlay on top of the game state stack.
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
 * Core engine driver.
 */
class Core {
  public:
	/**
	 * Create a new core with an initial_state.
	 * @param initial_state The initial state of the game.
	 */
	Core(std::unique_ptr<IGameState> initial_state);

	/**
	 * Run the game.
	 * Update states and pass them input. Push and pop states based on state
	 * transitions.
	 */
	void run();

  private:
	std::unique_ptr<Window>                  m_window;
	std::vector<std::unique_ptr<IGameState>> m_gamestates;
	//	std::unordered_map<std::string, std::unique_ptr<IOverlay>> m_overlays;
};