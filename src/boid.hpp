#pragma once

#include "glm/fwd.hpp"
#include <p6/p6.h>
#include <vector>

class Boid {
private:
  glm::vec2 pos;
  glm::vec2 vel;
  float radius = 0.1f;
  float repulse_threshold = 0.4f;
  float repulse_power = 1.f;

public:
  Boid();

  glm::vec2 move();
  void avoid(const std::vector<Boid> &otherBoids);
  float getRadius() const { return radius; };
};
