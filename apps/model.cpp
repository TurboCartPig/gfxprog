/**
 * Lab 5 - Models and lighting.
 *
 * @author Dennis Kristiansen, NTNU
 * @file texture.cpp
 */

#include <glove/Components.h>
#include <glove/lib.h>
#include <iostream>

auto main() -> int {
	auto window = Window("Model", 1280, 720);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable blending for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	auto shader_program = ShaderProgram(
	    {"resources/shaders/model.vert", "resources/shaders/model.frag"});
	shader_program.use();

	auto transform = glm::mat4(1.0f);

	auto [w, h] = window.dimensions();
	auto aspect = (float)w / (float)h;

	glm::quat          q = glm::quatLookAt(glm::vec3(0.0f, 0.0f, -1.0f),
                                  glm::vec3(0.0f, 1.0f, 0.0f));
	TransformComponent transform_component = {glm::vec3(0.0f, 1.0f, -2.0f), q,
	                                          glm::vec3(1.0f)};
	CameraComponent    camera(aspect, 96.0f);
	auto               view       = camera.view(transform_component);
	auto               projection = camera.projection();

	auto model_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	auto directional_light = DirectionalLight{
	    glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 5.0f), 10.0f};

	shader_program.setUniform("u_transform", transform);
	shader_program.setUniform("u_view", view);
	shader_program.setUniform("u_projection", projection);
	shader_program.setUniform("u_model_color", model_color);
	shader_program.setUniform("u_directional_light", directional_light);
	shader_program.setUniform("u_diffuse_map", 0u);

	auto model = Model("resources/models/teacup.obj");

	while (!window.pollEvents()) {
		transform = glm::rotate(transform, window.getDeltaTime() * 0.1f,
		                        glm::vec3(0.0f, 1.0f, 0.0f));
		shader_program.setUniform("u_transform", transform);

		model.draw();

		window.swapBuffers();
	}

	return EXIT_SUCCESS;
}
