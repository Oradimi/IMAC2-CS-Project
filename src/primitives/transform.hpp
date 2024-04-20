#pragma once

#include <glm/glm.hpp>

/// A class that creates a model matrix
class Transform {
private:
  glm::vec3 position;
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

  glm::mat4 getTransform() const { return transform; };
};

// Doesn't work correctly
// glm::vec3 velocityToRotationVector(const glm::vec3 &velocity,
//                                    const glm::vec3 &angle_offset);
glm::mat4 computeRotationMatrix(const glm::vec3 &velocity);
