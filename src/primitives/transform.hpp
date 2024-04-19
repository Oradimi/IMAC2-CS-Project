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

  Transform(glm::vec3 position, glm::vec3 rotation, float uniformScale = 1.f);

  glm::mat4 getTransform() const { return transform; };
};

glm::vec3 velocityToRotationVector(const glm::vec3 &velocity);