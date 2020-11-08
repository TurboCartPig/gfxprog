#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

struct TransformComponent {
  public:
	TransformComponent() = default;

  public:
	glm::vec3 translation;
	glm::quat rotation;
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

struct MeshComponent {};