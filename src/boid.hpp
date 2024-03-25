#pragma once

#include "glm/fwd.hpp"
#include <p6/p6.h>
#include <vector>

glm::vec2 clampVectorSpeed(glm::vec3 vec);

class Boid {
private:
  static float turn_factor;
  static float vision_range;
  static float protected_range;
  static float centering_factor;
  static float avoid_factor;
  static float matching_factor;
  static float max_speed;
  static float min_speed;
  static float max_bias;
  static float bias_increment;

  glm::vec3 pos;
  glm::vec3 vel;

public:
  Boid();

  static void initializeUIElements();

  void separation(const std::vector<Boid> &swarm);
  void alignment(const std::vector<Boid> &swarm);
  void cohesion(const std::vector<Boid> &swarm);
  void move();

  glm::vec3 getPosition() const { return pos; };
  glm::vec3 getVelocity() const { return vel; };
};
