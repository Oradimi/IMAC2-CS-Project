#include "transform.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <utility>

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
    : transform(
          glm::translate(glm::mat4{1.f},
                         glm::vec3{position.x, position.y, position.z}) *
          glm::rotate(glm::mat4{1.f}, glm::radians(rotation.x),
                      glm::vec3{1.f, 0.f, 0.f}) *
          glm::rotate(glm::mat4{1.f}, glm::radians(rotation.y),
                      glm::vec3{0.f, 1.f, 0.f}) *
          glm::rotate(glm::mat4{1.f}, glm::radians(rotation.z),
                      glm::vec3{0.f, 0.f, 1.f}) *
          glm::scale(glm::mat4{1.f}, glm::vec3{scale.x, scale.y, scale.z})){};
