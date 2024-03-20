#pragma once

#include "glm/fwd.hpp"
#include <p6/p6.h>
#include <vector>

glm::vec2 clampVectorSpeed(glm::vec3 vec);

class Boid {
private:
  glm::vec3 pos;
  glm::vec3 vel;
  float radius = 0.1f;
  float repulse_threshold = 0.4f;
  float repulse_power = 1.f;

public:
  Boid();

  glm::vec3 move();
  void avoid(const std::vector<Boid> &otherBoids);
  float getRadius() const { return radius; };
  p6::Angle getAngle() const { return p6::Angle(vel); };
};
