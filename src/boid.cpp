#pragma once

#include "boid.hpp"
#include "glm/fwd.hpp"
#include <cstdlib>
#include <p6/p6.h>
#include <vector>

glm::vec2 clampVectorSpeed(glm::vec2 vec, float max) {
  // std::cout << max / glm::length(vec) << std::endl;
  if (glm::length(vec) > max)
    return {vec.x * max / glm::length(vec), vec.y * max / glm::length(vec)};
  return vec;
}

Boid::Boid() {
  pos = {static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f),
         static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f)};

  vel = {static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f),
         static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f)};
  vel *= 200.f;
  // std::cout << pos.x << " " << pos.y << std::endl;
  // std::cout << vel.x << " " << vel.y << std::endl;
}

glm::vec2 Boid::move() {
  pos += vel;
  vel *= 0.999;
  float max_speed = 0.02f;
  vel = clampVectorSpeed(vel, max_speed);
  if (pos.x > 1.0f + radius) {
    pos.x = -1.0f - radius;
  } else if (pos.x < -1.0f - radius) {
    pos.x = 1.0f + radius;
  }

  if (pos.y > 1.0f + radius) {
    pos.y = -1.0f - radius;
  } else if (pos.y < -1.0f - radius) {
    pos.y = 1.0f + radius;
  }
  return pos;
}

void Boid::avoid(const std::vector<Boid> &otherBoids) {
  glm::vec2 avoidanceDir{0.0f, 0.0f};
  float min_distance = 4.f;
  for (const Boid &otherBoid : otherBoids) {
    if (&otherBoid == this)
      continue;

    float distance = glm::distance(pos, otherBoid.pos);
    if (distance < min_distance) {
      min_distance = glm::distance(pos, otherBoid.pos);
      avoidanceDir = pos - otherBoid.pos;
    }
  }
  vel += avoidanceDir * (radius + repulse_threshold - min_distance) * 0.01f *
         repulse_power;
}
