#pragma once

#include <GLFW/glfw3.h>
#include <string>

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
	Window(const std::string& title, const uint32_t width, const uint32_t height);

	~Window();

	/**
	 * Poll for new events and indicate if the window should close.
	 * @return Should the window be closed?
	 */
	bool pollEvents();

	/**
	 * Swap the back and front buffers.
	 */
	void swapBuffers();

  private:
	GLFWwindow *m_window;
};