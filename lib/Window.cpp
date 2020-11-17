#include <GL/glew.h>
#include <functional>
#include <glove/Window.h>
#include <glove/lib.h>
#include <iostream>

static std::function<void(GLFWwindow *, int, int, int, int)>
    gKeyCallback; ///< Part of hack to allow the key callback to capture the
                  ///< environment

static std::function<void(GLFWwindow *, int, int)> gFramebufferResizeCallback;

/**
 * Convert from glfw key codes to gloves key codes.
 *
 * FIXME: Add the rest of the keyboard.
 *
 * @param key GLFW key code.
 * @return Glove key code.
 */
static auto glfw_to_inputcode(int key) {
	switch (key) {
		case GLFW_KEY_Q: return InputCode::Q;
		case GLFW_KEY_W: return InputCode::W;
		case GLFW_KEY_E: return InputCode::E;
		case GLFW_KEY_R: return InputCode::R;
		case GLFW_KEY_T: return InputCode::T;
		case GLFW_KEY_Y: return InputCode::Y;
		case GLFW_KEY_U: return InputCode::U;
		case GLFW_KEY_I: return InputCode::I;
		case GLFW_KEY_O: return InputCode::O;
		case GLFW_KEY_P: return InputCode::P;
		case GLFW_KEY_A: return InputCode::A;
		case GLFW_KEY_S: return InputCode::S;
		case GLFW_KEY_D: return InputCode::D;
		case GLFW_KEY_F: return InputCode::F;
		case GLFW_KEY_G: return InputCode::G;
		case GLFW_KEY_H: return InputCode::H;
		case GLFW_KEY_J: return InputCode::J;
		case GLFW_KEY_K: return InputCode::K;
		case GLFW_KEY_L: return InputCode::L;
		case GLFW_KEY_Z: return InputCode::Z;
		case GLFW_KEY_X: return InputCode::X;
		case GLFW_KEY_C: return InputCode::C;
		case GLFW_KEY_V: return InputCode::V;
		case GLFW_KEY_B: return InputCode::B;
		case GLFW_KEY_N: return InputCode::N;
		case GLFW_KEY_M: return InputCode::M;
		case GLFW_KEY_ESCAPE: return InputCode::Escape;
		case GLFW_KEY_UP: return InputCode::Up;
		case GLFW_KEY_DOWN: return InputCode::Down;
		case GLFW_KEY_LEFT: return InputCode::Left;
		case GLFW_KEY_RIGHT: return InputCode::Right;
	}
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
	m_input_queue = std::make_shared<std::deque<Input>>();

	// Setup key callback
	auto input = m_input_queue; // Get a copy of the input queue pointer for
	                            // capture by lambda
	auto callback = [input]([[maybe_unused]] GLFWwindow *window, int key,
	                        [[maybe_unused]] int scancode, int action,
	                        [[maybe_unused]] int modes) mutable -> void {
		InputCode  code = glfw_to_inputcode(key);
		InputState state;

		switch (action) {
			case GLFW_PRESS: state = InputState::Pressed; break;
			case GLFW_RELEASE: state = InputState::Released; break;
			case GLFW_REPEAT: state = InputState::Held; break;
			default: throw std::runtime_error("Unknown GLFW action!");
		}

		auto i = Input{code, state};
		input->push_back(i);
	};

	// Hack to make the key callback to work with lambda's that capture the
	// environment
	gKeyCallback = std::move(callback);
	glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode,
	                                int action, int modes) {
		gKeyCallback(window, key, scancode, action, modes);
	});

	// Set temporary empty callback
    gFramebufferResizeCallback = std::move([](GLFWwindow *window, int width, int height){});
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height){
		gFramebufferResizeCallback(window, width, height);
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
	glDebugMessageCallback(glDebugOutput, nullptr);

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

void Window::registerFramebufferResizeCallback(
    std::function<void(GLFWwindow *, int, int)> callback) {
	gFramebufferResizeCallback = std::move(callback);
}
