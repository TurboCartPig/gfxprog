#pragma once

#include <glove/Window.h>
#include <memory>
#include <variant>
#include <vector>

/**
 * @brief No state transition should occur.
 */
struct [[nodiscard]] None {};

/**
 * @brief Pop the current state off the stack.
 * If there are other states on the stack, the top one will be the next active
 * state. If there are no other states on the stack, the application will close.
 */
struct [[nodiscard]] Pop {};

/**
 * @brief Push a new state to the top of the stack.
 * This new state will be the next active state.
 */
struct [[nodiscard]] Push {
	std::unique_ptr<class IGameState> state;
};

/**
 * @brief Transition laterally to a new state. Think of it as a pop and push in
 * one operation.
 */
struct [[nodiscard]] Transition {
	std::unique_ptr<class IGameState> state;
};

/**
 * @brief Should the current state transition into a new state? If so how?
 */
using StateTransition = std::variant<None, Push, Pop, Transition>;

/**
 * @brief A manifest of static information about a game state.
 */
struct StateManifest {
	std::string title; ///< The game state's title.
};

/**
 * @brief Interface for a game state object.
 */
class IGameState {
  public:
	IGameState() = default;

	IGameState(const IGameState &other) = delete;

	IGameState(const IGameState &&other) = delete;

	auto operator=(const IGameState &other) = delete;

	auto operator=(const IGameState &&other) = delete;

	virtual ~IGameState() = default;

	/**
	 * @brief Initialize the game state.
	 */
	virtual void initialize() = 0;

	/**
	 * @brief Get a manifest of static information about this game state.
	 * @return Manifest.
	 */
	virtual auto manifest() -> StateManifest = 0;

	/**
	 * @brief Handle window and default framebuffer resize.
	 * @param width The new width of the default framebuffer.
	 * @param height The new height of the default framebuffer.
	 */
	virtual void resized(int width, int height) = 0;

	/**
	 * @brief Pass player input to the game state.
	 * @param input Input event.
	 * @return Should the game transition states as a result of the input? If so
	 * how?
	 */
	virtual auto input(Input input) -> StateTransition = 0;

	/**
	 * @brief Drive the game state forward.
	 * @param dt Delta time.
	 * @return Should the game transition states as a result of the input? If so
	 * how?
	 */
	virtual auto update(float dt) -> StateTransition = 0;

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
	IOverlay()                       = default;
	IOverlay(const IOverlay &other)  = delete;
	IOverlay(const IOverlay &&other) = delete;
	auto operator=(const IOverlay &other) = delete;
	auto operator=(const IOverlay &&other) = delete;
	virtual ~IOverlay()                    = default;
	virtual void attach()                  = 0;
	virtual void detach()                  = 0;
	virtual void input(Input input)        = 0;
	virtual void update(float dt)          = 0;
	virtual void render()                  = 0;
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

	/**
	 * @brief Sets up and initializes the new top state.
	 */
	void setupState();

  private:
	std::unique_ptr<Window>                  m_window;
	std::vector<std::unique_ptr<IGameState>> m_gamestates;
	//	std::unordered_map<std::string, std::unique_ptr<IOverlay>> m_overlays;
};
