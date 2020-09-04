#include <GL/glew.h>
#include <chrono>
#include <cmath>
#include <glove/lib.h>
#include <iostream>

using std::chrono::duration;
using std::chrono::steady_clock;

int main() {
	auto window = Window("Square", 640, 480);

	std::vector<Vertex2D> vertices = {
	    Vertex2D{-0.5f, -0.5f}, Vertex2D{-0.5f, 0.5f}, Vertex2D{0.5f, -0.5f},
	    Vertex2D{0.5f, 0.5f}};
	std::vector<GLuint> indices = {0, 1, 2, 1, 2, 3};

	auto vbo = VertexBufferObject(vertices, indices);
	auto program =
	    ShaderProgram({"shaders/shader.vert", "shaders/shader.frag"});

	auto start = steady_clock::now();

	while (!window.pollEvents()) {
		auto             now = steady_clock::now();
		duration<double> dur = now - start;
		auto             ggg = (dur.count() - std::floor(dur.count())) / 3.0f;

		program.use();
		program.setUniform("ui_color", glm::vec3(0.0f, ggg, 0.0f));
		vbo.draw();

		window.swapBuffers();
	}

	return EXIT_SUCCESS;
}