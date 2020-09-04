#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <glove/lib.h>
#include <iostream>

int main() {
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
	GLFWwindow *window =
	    glfwCreateWindow(1280, 720, "Triangle", nullptr, nullptr);
	if (!window) {
		throw std::runtime_error("GLFW3 Failed to create window.");
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode,
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

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl
	          << "Renderer: " << glGetString(GL_RENDERER) << std::endl
	          << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Setup a triangle
	// 1. Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 2. Vertex Buffer Object
	std::array<Vertex2DRgb, 3> content = {
	    Vertex2DRgb{-0.6f, -0.4f, 0.0f, 1.0f, 0.0f},
	    Vertex2DRgb{0.6f, -0.4f, 0.0f, 0.0f, 1.0f},
	    Vertex2DRgb{0.0f, 0.6f, 1.0f, 0.0f, 0.0f}};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, content.size() * sizeof(Vertex2DRgb),
	             content.data(), GL_STATIC_DRAW);

	// 3. Attributes
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DRgb), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DRgb),
	                      (void *)(sizeof(float) * 2));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// 4. Shaders
	ShaderProgram program({"shaders/triangle.vert", "shaders/triangle.frag"});

	// Main game loop
	while (!glfwWindowShouldClose(window)) {
		// Set viewport in case it changed
		int height, width;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Clear framebuffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		program.use();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Present framebuffer and poll new window events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
