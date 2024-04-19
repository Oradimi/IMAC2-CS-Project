#include "transform.hpp"
#include "glm/gtx/quaternion.hpp"

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : position(position), rotation(rotation), scale(scale),
      transform(glm::translate(glm::mat4{1.f}, position) *
                glm::rotate(glm::mat4{1.f}, glm::radians(rotation.x),
                            glm::vec3{1.f, 0.f, 0.f}) *
                glm::rotate(glm::mat4{1.f}, glm::radians(rotation.y),
                            glm::vec3{0.f, 1.f, 0.f}) *
                glm::rotate(glm::mat4{1.f}, glm::radians(rotation.z),
                            glm::vec3{0.f, 0.f, 1.f}) *
                glm::scale(glm::mat4{1.f}, scale)){};

Transform::Transform(glm::vec3 position, glm::vec3 rotation, float uniformScale)
    : position(position), rotation(rotation), scale(glm::vec3{uniformScale}),
      transform(glm::translate(glm::mat4{1.f}, position) *
                glm::rotate(glm::mat4{1.f}, glm::radians(rotation.x),
                            glm::vec3{1.f, 0.f, 0.f}) *
                glm::rotate(glm::mat4{1.f}, glm::radians(rotation.y),
                            glm::vec3{0.f, 1.f, 0.f}) *
                glm::rotate(glm::mat4{1.f}, glm::radians(rotation.z),
                            glm::vec3{0.f, 0.f, 1.f}) *
                glm::scale(glm::mat4{1.f}, scale)){};

glm::vec3 velocityToRotationVector(const glm::vec3 &velocity) {
  glm::vec3 direction = glm::normalize(velocity);

  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::quat rotation = glm::rotation(up, direction);
  glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(rotation));

  return eulerAngles;
};