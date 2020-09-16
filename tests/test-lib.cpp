#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <glove/lib.h>

/**
 * Test if loading and compiling shaders doesn't throw exceptions or crash
 */
TEST_CASE("Compile Shader Programs", "[shaders]") {
	auto window = Window("Test", 640, 480);

	SECTION("Triangle shaders") {
		auto shader =
		    ShaderProgram({"shaders/triangle.vert", "shaders/triangle.frag"});
		shader.use();
	}

	SECTION("Square shaders") {
		auto shader =
		    ShaderProgram({"shaders/square.vert", "shaders/square.frag"});
		shader.use();
		shader.setUniform("u_color", glm::vec3(1.0f, 1.0f, 1.0f));
	}

	SECTION("MVP shaders") {
		auto shader = ShaderProgram({"shaders/mvp.vert", "shaders/mvp.frag"});
		shader.use();
		shader.setUniform("u_transform", glm::mat4(1.0f));
		shader.setUniform("u_view", glm::mat4(1.0f));
		shader.setUniform("u_projection", glm::mat4(1.0f));
	}
}
