#include <glove/lib.h>
#include <GL/glew.h>
#include <iostream>

int main() {
	auto window = Window("Square", 640, 480);

	std::vector<std::string> paths    = {"./apps/triangle.frag",
                                      "./apps/triangle.vert"};
	std::vector<Vertex2DRgb> vertices = {
	    Vertex2DRgb{-0.5f, -0.5f, 1.0f, 1.0f, 1.0f},
	    Vertex2DRgb{-0.5f, 0.5f, 1.0f, 1.0f, 1.0f},
	    Vertex2DRgb{0.5f, -0.5f, 1.0f, 1.0f, 1.0f},
	    Vertex2DRgb{0.5f, 0.5f, 1.0f, 1.0f, 1.0f}};
	std::vector<GLuint> indices = {0, 1, 2, 1, 2, 3};

	auto vbo     = VertexBufferObject(vertices, indices);
	auto program = ShaderProgram(paths);

	while (!window.pollEvents()) {
		program.use();
		vbo.draw();

		window.swapBuffers();
	}

	return EXIT_SUCCESS;
}