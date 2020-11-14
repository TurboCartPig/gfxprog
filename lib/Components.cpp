#include <glove/Components.h>

CameraComponent::CameraComponent(float aspect, float vfov)
    : aspect(aspect), vfov(vfov) {}

auto CameraComponent::view(const TransformComponent &transform) const
    -> glm::mat4 {
	auto center =
	    transform.translation +
	    glm::vec3(glm::eulerAngleXYZ(transform.rotation.x, transform.rotation.y,
	                                 transform.rotation.z) *
	              glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	return glm::lookAt(transform.translation, center,
	                   glm::vec3(0.0f, 1.0f, 0.0f));
}

auto CameraComponent::projection() const -> glm::mat4 {
	return glm::perspective(glm::radians(vfov), aspect, 0.0001f, 100.0f);
}