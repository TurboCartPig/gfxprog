#pragma once

#include <glm/gtx/euler_angles.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

struct TransformComponent {
  public:
	TransformComponent() = default;

	[[nodiscard]] auto matrix() const {
		return glm::translate(glm::mat4(1.0f), translation) *
		       glm::scale(glm::mat4(1.0f), scale) *
		       glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
	}

  public:
	glm::vec3 translation;
	glm::vec3 rotation; ///< Rotation in euler angles.
	glm::vec3 scale;
};

struct CameraComponent {
  public:
	CameraComponent() = default; // FIXME: This is invalid
	CameraComponent(float aspect, float vfov);

	[[nodiscard]] auto view(const TransformComponent &transform) const
	    -> glm::mat4;

	[[nodiscard]] auto projection() const -> glm::mat4;

  public:
	float aspect; ///< Aspect ratio.
	float vfov;   ///< Vertical field of view in degrees.
};
