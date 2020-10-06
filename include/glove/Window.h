#pragma once

#include <GLFW/glfw3.h>
#include <deque>
#include <memory>
#include <string>

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
	// TODO: Add the rest of a normal keyboard
};

using InputQueue = std::shared_ptr<std::deque<InputCode>>;

/**
 * Window object that maintains a window and opengl context.
 */
class Window {
  public:
	/**
	 * Create a new window and opengl context.
	 * @param title Title of the window.
	 * @param width Width of the window.
	 * @param height Height of the window.
	 */
	Window(const std::string &title, uint32_t width, uint32_t height);

	~Window();

	/**
	 * Poll for new events and indicate if the window should closed.
	 * @return Should the window be closed?
	 */
	bool pollEvents();

	/**
	 * Swap the back and front buffers.
	 */
	void swapBuffers();

	/**
	 * Get the internal window pointer for usage width unwrapped glfw calls.
	 * @return The internal window pointer.
	 */
	GLFWwindow *ptr() const { return m_window; }

	/**
	 * Get the current window dimensions without accounting for DPI. i.e.
	 * framebuffer size.
	 * @return Dimensions.
	 */
	std::pair<int, int> dimensions() const {
		auto dim = std::make_pair(0, 0);
		glfwGetFramebufferSize(m_window, &dim.first, &dim.second);
		return dim;
	}

	/**
	 * Set the dimensions of the window.
	 * @param width New width of window.
	 * @param height New height of window.
	 */
	void setDimensions(const int width, const int height) {
		glfwSetWindowSize(m_window, width, height);
	}

	/**
	 * Get a shared pointer to the input queue.
	 * @return A shared pointer to the input queue.
	 */
	InputQueue getInputQueue() { return m_input_queue; }

  private:
	GLFWwindow *m_window;
	InputQueue  m_input_queue;
};