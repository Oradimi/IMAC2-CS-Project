#pragma once

#include "glm/fwd.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <glm/glm.hpp>
#include <iostream>

/// A class that creates a model matrix
class Transform {
private:
  glm::vec3 position;
  glm::vec3 targetPosition;
  glm::vec3 rotation;
  glm::vec3 scale;

  glm::mat4 transform;

public:
  explicit Transform(glm::vec3 position = glm::vec3{0.f},
                     glm::vec3 rotation = glm::vec3{0.f},
                     glm::vec3 scale = glm::vec3{1.f});

  explicit Transform(glm::vec3 position = glm::vec3{0.f},
                     glm::vec3 rotation = glm::vec3{0.f},
                     float uniformScale = 1.f);

  void updateModelMatrix() {
    transform =
        glm::translate(glm::mat4{1.f}, position) *
        glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x),
                           glm::radians(rotation.z)) *
        glm::scale(glm::mat4{1.f}, scale);
  }

  glm::mat4 temporarySnap(glm::vec3 &vector) {
    transform =
        glm::translate(glm::mat4{1.f}, position + vector) *
        glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x),
                           glm::radians(rotation.z)) *
        glm::scale(glm::mat4{1.f}, scale);
    return transform;
  }

  void move(glm::vec3 vector) {
    position += vector;
    updateModelMatrix();
  }

  void moveEase(glm::vec3 vector) { targetPosition += vector; }

  void easeToTargetPosition(float deltaTime, float easingFactor);

  glm::mat4 getTransform() const { return transform; }

  glm::vec3 getPosition() const { return position; }
};

// Doesn't work correctly
// glm::vec3 velocityToRotationVector(const glm::vec3 &velocity,
//                                    const glm::vec3 &angle_offset);
glm::mat4 computeRotationMatrix(const glm::vec3 &velocity);
