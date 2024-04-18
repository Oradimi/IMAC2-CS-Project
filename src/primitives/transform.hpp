#pragma once

#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include "texture.hpp"
#include <glm/glm.hpp>
#include <vector>

/// A class that creates a model matrix
class Transform {
private:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  glm::mat4 transform;

public:
  Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

  glm::mat4 getTransform() const { return transform; };
};