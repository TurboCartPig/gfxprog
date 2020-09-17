#include <chrono>
#include <glove/lib.h>
#include <iostream>

using std::chrono::steady_clock;

int main() {
	auto window = Window("MVP", 640, 480);

	std::vector<Vertex2D> vertices = {
	    Vertex2D{-0.5f, -0.5f}, Vertex2D{-0.5f, 0.5f}, Vertex2D{0.5f, -0.5f},
	    Vertex2D{0.5f, 0.5f}};
	std::vector<GLuint> indices = {0, 1, 2, 1, 2, 3};

	auto vbo = VertexBufferObject(vertices, indices);

	auto program = ShaderProgram({"shaders/mvp.vert", "shaders/mvp.frag"});
	program.use();

	// Create matrices
	auto transform = glm::mat4(1.0f);

	auto view =
	    glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	                glm::vec3(0.0f, 1.0f, 0.0f));

	auto projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);

	// Setup clock for time measurements
	auto start = steady_clock::now();

	while (!window.pollEvents()) {
		auto  now = steady_clock::now();
		float dt  = (now - start).count() / 1000000000.0f;
		start     = now;

		// Get input
		float x = 0.0f, y = 0.0f, rot = 0.0f;
		if (glfwGetKey(window.ptr(), GLFW_KEY_W) == GLFW_PRESS) {
			y = 1.0f;
		}
		if (glfwGetKey(window.ptr(), GLFW_KEY_S) == GLFW_PRESS) {
			y = -1.0f;
		}
		if (glfwGetKey(window.ptr(), GLFW_KEY_D) == GLFW_PRESS) {
			x = 1.0f;
		}
		if (glfwGetKey(window.ptr(), GLFW_KEY_A) == GLFW_PRESS) {
			x = -1.0f;
		}
		if (glfwGetKey(window.ptr(), GLFW_KEY_Q) == GLFW_PRESS) {
			rot = -1.0f;
		}
		if (glfwGetKey(window.ptr(), GLFW_KEY_E) == GLFW_PRESS) {
			rot = 1.0f;
		}

		// Translate the camera based on input
		transform = glm::translate(transform, glm::vec3(dt * x, dt * y, 0.0f));

		transform =
		    glm::rotate(transform, dt * rot, glm::vec3(0.0f, 0.0f, -1.0f));

		// Update projection to account for the framebuffer's aspect ratio
		auto [w, h] = window.dimensions();
		auto ar     = (float)w / (float)h;
		projection  = glm::ortho(-ar, ar, -1.0f, 1.0f);

		// Update to uniforms
		program.setUniform("u_transform", transform);
		program.setUniform("u_view", view);
		program.setUniform("u_projection", projection);

		vbo.draw();

		window.swapBuffers();
	}

	return EXIT_SUCCESS;
}