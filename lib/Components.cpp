#include <glove/Components.h>
#include <glove/utility.h>

CameraComponent::CameraComponent(float aspect, float vfov)
    : aspect(aspect), vfov(vfov) {}

glm::mat4
CameraComponent::asViewProjection(const TransformComponent &transform) const {
	auto projection =
	    glm::perspective(glm::radians(vfov), aspect, 0.01f, 100.0f);

	auto center = transform.translation + transform.rotation * glm::vec3(0.0f, 0.0f, 1.0f);
	std::cout << "Center: " << center << std::endl;
	auto view = glm::lookAt(transform.translation,
	                        center,
	                        glm::vec3(0.0f, 1.0f, 0.0f));

	return projection * view;
}
