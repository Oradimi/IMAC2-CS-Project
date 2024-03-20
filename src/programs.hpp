#pragma once

#include "glimac/cone_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "p6/p6.h"
#include "primitives/object.hpp"
#include "primitives/texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

inline std::vector<glm::vec3> createRandomAxes() {
  std::vector<glm::vec3> randomAxes{glm::sphericalRand(2.f)};
  for (int i = 1; i < 32; i++) {
    randomAxes.emplace_back(glm::sphericalRand(2.f));
  }
  return randomAxes;
};
