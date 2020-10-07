#include <GL/glew.h>
#include <functional>
#include <glove/Window.h>
#include <glove/lib.h>
#include <iostream>

static std::function<void(GLFWwindow *, int, int, int, int)>
    gKeyCallback; ///< Part of hack to allow the key callback to capture the
                  ///< environment

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
	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!m_window) {
		throw std::runtime_error("GLFW3 Failed to create window.");
	}

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	// Setup input queue
	m_input_queue = std::make_shared<std::deque<InputCode>>();

	// Setup key callback
	auto input = m_input_queue; // Get a copy of the input queue pointer for
	                            // capture by lambda
	auto callback = [input](GLFWwindow *window, int key, int scancode,
	                        int action, int modes) mutable -> void {
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			} else if (key == GLFW_KEY_F11) {
				const auto monitor = glfwGetPrimaryMonitor();
				const auto mode    = glfwGetVideoMode(monitor);
				glfwSetWindowMonitor(window, monitor, 0, 0, mode->width,
				                     mode->height, mode->refreshRate);
			} else {
				switch (key) {
					// TODO: More keys
					case GLFW_KEY_W: input->push_back(InputCode::W); break;
					case GLFW_KEY_S: input->push_back(InputCode::S); break;
					case GLFW_KEY_A: input->push_back(InputCode::A); break;
					case GLFW_KEY_D: input->push_back(InputCode::D); break;
				}
			}
		}
	};

	// Hack to make the key callback to work with lambda's that capture the
	// environment
	gKeyCallback = std::move(callback);
	glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode,
	                                int action, int modes) {
		gKeyCallback(window, key, scancode, action, modes);
	});

	// Setup GLEW
	glewExperimental = GL_TRUE;
	GLenum err       = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		throw std::runtime_error("GLEW Failed to initialize.");
	}

	// Enable debugging
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
	// Clear the input queue to avoid storing all the input events for the
	// entire game
	m_input_queue->clear();

	// THEN poll events
	glfwPollEvents();

	return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers() {
	glfwSwapBuffers(m_window);

	glClear(GL_COLOR_BUFFER_BIT);
}
