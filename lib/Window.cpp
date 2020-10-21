#include <GL/glew.h>
#include <functional>
#include <glove/Window.h>
#include <glove/lib.h>
#include <iostream>

static std::function<void(GLFWwindow *, int, int, int, int)>
    gKeyCallback; ///< Part of hack to allow the key callback to capture the
                  ///< environment

/**
 * Convert from glfw key codes to gloves key codes.
 *
 * FIXME: Add the rest of the keyboard.
 *
 * @param key GLFW key code.
 * @return Glove key code.
 */
static InputCode glfw_to_inputcode(int key) {
	InputCode ret;

	switch (key) {
		case GLFW_KEY_Q: ret = InputCode::Q; break;
		case GLFW_KEY_W: ret = InputCode::W; break;
		case GLFW_KEY_E: ret = InputCode::E; break;
		case GLFW_KEY_R: ret = InputCode::R; break;
		case GLFW_KEY_T: ret = InputCode::T; break;
		case GLFW_KEY_Y: ret = InputCode::Y; break;
		case GLFW_KEY_U: ret = InputCode::U; break;
		case GLFW_KEY_I: ret = InputCode::I; break;
		case GLFW_KEY_O: ret = InputCode::O; break;
		case GLFW_KEY_P: ret = InputCode::P; break;
		case GLFW_KEY_A: ret = InputCode::A; break;
		case GLFW_KEY_S: ret = InputCode::S; break;
		case GLFW_KEY_D: ret = InputCode::D; break;
		case GLFW_KEY_F: ret = InputCode::F; break;
		case GLFW_KEY_G: ret = InputCode::G; break;
		case GLFW_KEY_H: ret = InputCode::H; break;
		case GLFW_KEY_J: ret = InputCode::J; break;
		case GLFW_KEY_K: ret = InputCode::K; break;
		case GLFW_KEY_L: ret = InputCode::L; break;
		case GLFW_KEY_Z: ret = InputCode::Z; break;
		case GLFW_KEY_X: ret = InputCode::X; break;
		case GLFW_KEY_C: ret = InputCode::C; break;
		case GLFW_KEY_V: ret = InputCode::V; break;
		case GLFW_KEY_B: ret = InputCode::B; break;
		case GLFW_KEY_N: ret = InputCode::N; break;
		case GLFW_KEY_M: ret = InputCode::M; break;
		case GLFW_KEY_ESCAPE: ret = InputCode::Escape; break;
	}

	return ret;
}

Window::Window(const std::string &title, const uint32_t width,
               const uint32_t height) {
	// Initialize glfw
	if (glfwInit() == GLFW_FALSE) {
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
	glfwWindowHint(GLFW_SAMPLES, 4);
	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (m_window == nullptr) {
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
			input->push_back(glfw_to_inputcode(key));
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

	auto now     = std::chrono::steady_clock::now();
	m_delta_time = (now - m_prev_frame).count() / 1000000000.0f;
	m_prev_frame = now;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::fullscreen(bool should_be_fullscreen) {
	const auto monitor = glfwGetPrimaryMonitor();
	const auto mode    = glfwGetVideoMode(monitor);
	glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height,
	                     mode->refreshRate);
}
