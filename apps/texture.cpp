/**
 * Lab 4 - Textures.
 *
 * @author Dennis Kristiansen, NTNU
 * @file texture.cpp
 */

#include <glove/lib.h>
#include <iostream>

int main() {
	auto window = Window("Square", 640, 480);

	std::vector<Vertex2DTex> vertices = {
	    Vertex2DTex{glm::vec2(-0.5f, -0.5f), glm::vec2(1.0f, 1.0f)},
	    Vertex2DTex{glm::vec2(-0.5f, 0.5f), glm::vec2(1.0f, 0.0f)},
	    Vertex2DTex{glm::vec2(0.5f, -0.5f), glm::vec2(0.0f, 1.0f)},
	    Vertex2DTex{glm::vec2(0.5f, 0.5f), glm::vec2(0.0f, 0.0f)}};
	std::vector<GLuint> indices = {0, 1, 2, 1, 2, 3};

	auto program = ShaderProgram(
	    {"resources/shaders/texture.vert", "resources/shaders/texture.frag"});
	auto vbo = VertexBuffer(vertices, indices);

	// Enable blending for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	auto tex = Texture("resources/textures/dog.png");
	tex.bindToSlot(0);

	while (!window.pollEvents()) {
		program.use();
		program.setUniform("u_texA", 0u);
		vbo.draw();

		window.swapBuffers();
	}

	return EXIT_SUCCESS;
}
