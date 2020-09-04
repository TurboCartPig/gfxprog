#include <GL/glew.h>
#include <glove/Window.h>
#include <glove/lib.h>
#include <iostream>

Window::Window(const std::string &title, const uint32_t width,
               const uint32_t height) {
	// Initialize glfw
    if (!glfwInit()) {
        throw std::runtime_error("GLFW3 Failed to initialize.");
    }

    // Setup error callback
    glfwSetErrorCallback([](int error, const char *desc) {
      std::cerr << "GLFW Error: " << error << " - " << desc << std::endl;
    });

    // Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    m_window =
        glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window) {
        throw std::runtime_error("GLFW3 Failed to create window.");
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode,
                                  int action, int modes) {
      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
          glfwSetWindowShouldClose(window, GLFW_TRUE);
      }
    });

    // Setup GLEW
    glewExperimental = GL_TRUE;
    GLenum err       = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        throw std::runtime_error("GLEW Failed to initialize.");
    }

    // Debug
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl
              << "Renderer: " << glGetString(GL_RENDERER) << std::endl
              << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
}

Window::~Window() {
    glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Window::pollEvents() {
	glfwPollEvents();
	return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);

    // Set viewport in case it changed
    int height, width;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT);
}
