#include "transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include <iostream>

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : position(position), targetPosition(position), rotation(rotation),
      scale(scale), transform(glm::translate(glm::mat4{1.f}, position) *
                              glm::eulerAngleYXZ(glm::radians(rotation.y),
                                                 glm::radians(rotation.x),
                                                 glm::radians(rotation.z)) *
                              glm::scale(glm::mat4{1.f}, scale)){};

Transform::Transform(glm::vec3 position, glm::vec3 rotation, float uniformScale)
    : position(position), targetPosition(position), rotation(rotation),
      scale(glm::vec3{uniformScale}),
      transform(glm::translate(glm::mat4{1.f}, position) *
                glm::eulerAngleYXZ(glm::radians(rotation.y),
                                   glm::radians(rotation.x),
                                   glm::radians(rotation.z)) *
                glm::scale(glm::mat4{1.f}, scale)){};

void Transform::easeToTargetPosition(float deltaTime, float easingFactor) {
  glm::vec3 direction = targetPosition - position;

  if (direction == glm::vec3{0.f})
    return;

  float distance = glm::length(direction);

  if (distance < 0.001f) {
    position = targetPosition;
  } else {
    float movementAmount = distance * easingFactor * deltaTime;
    movementAmount = glm::min(movementAmount, distance);
    position +=
        glm::clamp((direction / distance) * movementAmount, -0.5f, 0.5f);
  }

  updateModelMatrix();
}

// glm::vec3 velocityToRotationVector(const glm::vec3 &velocity,
//                                    const glm::vec3 &angle_offset) {
//   glm::vec3 direction = glm::normalize(velocity);

//   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//   glm::quat rotation = glm::rotation(up, direction);
//   glm::vec3 euler_angles = glm::degrees(glm::eulerAngles(rotation));

//   return euler_angles + glm::degrees(angle_offset);
// };

glm::mat4 computeRotationMatrix(const glm::vec3 &velocity) {
  glm::vec3 direction = glm::normalize(velocity);

  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::quat rotation = glm::rotation(up, direction);

  glm::mat4 rotationMatrix = glm::mat4_cast(rotation);

  return rotationMatrix;
};