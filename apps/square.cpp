/**
 * Draw a pulsing square with OpenGL and the utility library I wrote called
 * glove.
 *
 * @author Dennis Kristiansen, NTNU
 * @file square.cpp
 */

#include <chrono>
#include <glove/lib.h>
#include <iostream>

using std::chrono::duration;
using std::chrono::steady_clock;

auto main() -> int {
	auto window = Window("Square", 640, 480);

	std::vector<Vertex2D> vertices = {
	    Vertex2D{glm::vec2(-0.5f, -0.5f)}, Vertex2D{glm::vec2(-0.5f, 0.5f)},
	    Vertex2D{glm::vec2(0.5f, -0.5f)}, Vertex2D{glm::vec2(0.5f, 0.5f)}};
	std::vector<GLuint> indices = {0, 1, 2, 1, 2, 3};

	auto program = ShaderProgram(
	    {"resources/shaders/square.vert", "resources/shaders/square.frag"});
	auto vbo = VertexBuffer(vertices, indices);

	const auto start = steady_clock::now();

	while (!window.pollEvents()) {
		const auto       now = steady_clock::now();
		duration<double> dur = now - start;
		const auto       ggg = (dur.count() - std::floor(dur.count())) / 3.0f;

		program.use();
		program.setUniform("u_color", glm::vec3(0.0f, ggg, 0.0f));
		vbo.draw();

		window.swapBuffers();
	}

	return EXIT_SUCCESS;
}
