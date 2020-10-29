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

	/**
	 * Create a View Perspective Matrix from this camera to pass to shaders.
	 * @param transform Transform of the camera or entity that owns the camera.
	 * @return View Perspective Matrix.
	 */
	glm::mat4 asViewProjection(const TransformComponent &transform) const;

  public:
	float aspect; ///< Aspect ratio.
	float vfov;   ///< Vertical field of view in degrees.
};

struct MeshComponent {};