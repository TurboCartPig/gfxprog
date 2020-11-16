#pragma once

#include <GLFW/glfw3.h>
#include <chrono>
#include <deque>
#include <memory>
#include <string>

/**
 * @brief Input codes from keyboard.
 */
enum class InputCode {
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,
	Space,
	LCtrl,
	LShift,
	LAlt,
	RCtrl,
	RShift,
	RAlt,
	Tab,
	Escape,
	Up,
	Down,
	Left,
	Right,
	// TODO: Add the rest of a normal keyboard
};

/**
 * @brief State of an input code.
 */
enum class InputState {
	Pressed,
	Released,
	Held,
};

/**
 * @brief Input from the keyboard.
 */
struct Input {
	InputCode  code;
	InputState state;
};

using InputQueue = std::shared_ptr<std::deque<Input>>;

/**
 * @brief Window object that maintains a window and opengl context.
 */
class Window {
  public:
	/**
	 * @brief Create a new window and opengl context.
	 * @param title Title of the window.
	 * @param width Width of the window.
	 * @param height Height of the window.
	 */
	Window(const std::string &title, uint32_t width, uint32_t height);

	Window(const Window &other) = delete;

	Window(const Window &&other) = delete;

	auto operator=(const Window &other) = delete;

	auto operator=(const Window &&other) = delete;

	~Window();

	/**
	 * @brief Poll for new events and indicate if the window should closed.
	 * @return Should the window be closed?
	 */
	auto pollEvents() -> bool;

	/**
	 * @brief Swap the back and front buffers.
	 */
	void swapBuffers();

	/**
	 * @brief Get the internal window pointer for usage width unwrapped glfw
	 * calls.
	 * @return The internal window pointer.
	 */
	[[nodiscard]] auto ptr() const -> auto * { return m_window; }

	/**
	 * @brief Get the current window dimensions without accounting for DPI. i.e.
	 * framebuffer size.
	 * @return Dimensions.
	 */
	[[nodiscard]] auto dimensions() const {
		auto dim = std::make_pair(0, 0);
		glfwGetFramebufferSize(m_window, &dim.first, &dim.second);
		return dim;
	}

	/**
	 * @brief Set the dimensions of the window.
	 * @param width New width of window.
	 * @param height New height of window.
	 */
	void setDimensions(const int width, const int height) {
		glfwSetWindowSize(m_window, width, height);
	}

	/**
	 * @brief Set the title of the window.
	 * @param title New title.
	 */
	void setTitle(const std::string &title) {
		glfwSetWindowTitle(m_window, title.c_str());
	}

	/**
	 * @brief Should the window be fullscreened? Change current state if needed.
	 * @param should_be_fullscreen Should the window be fullscreened?
	 */
	void fullscreen(bool should_be_fullscreen);

	/**
	 * @brief Get the previous delta time.
	 * @return Delta time in seconds.
	 */
	[[nodiscard]] auto getDeltaTime() const -> float { return m_delta_time; }

	/**
	 * @brief Get a shared pointer to the input queue.
	 * @return A shared pointer to the input queue.
	 */
	[[nodiscard]] auto getInputQueue() { return m_input_queue; }

  private:
	std::chrono::steady_clock::time_point m_prev_frame;
	float                                 m_delta_time = 0.016f;
	GLFWwindow *                          m_window;
	InputQueue                            m_input_queue;
};