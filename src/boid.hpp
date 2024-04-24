#pragma once

#include "glm/fwd.hpp"
#include "primitives/math.hpp"
#include <p6/p6.h>
#include <vector>

void clampVectorMagnitude(glm::vec3 &vec, float min, float max);

class Boid {
private:
  static float bounds;
  static float turn_factor;
  static float protected_range;
  static float vision_range;
  static float avoid_factor;
  static float match_factor;
  static float center_factor;
  static float min_speed;
  static float max_speed;
  static float level_of_detail;
  static float max_bias;
  static float bias_increment;

  glm::vec3 pos{0.f, 0.f, 0.f};
  glm::vec3 vel{0.f, 0.f, 0.f};

public:
  explicit Boid(RandomMath &rand);

  static void initializeUIElements();

  void move(const std::vector<Boid> &swarm);
  void flock(const std::vector<Boid> &swarm);
  void avoidBounds();

  static float getBounds() { return bounds; };
  static float getLevelOfDetail() { return level_of_detail; };
  glm::vec3 getPosition() const { return pos; };
  glm::vec3 getVelocity() const { return vel; };
};
