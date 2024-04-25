#include "boids.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtx/norm.hpp"
#include <p6/p6.h>

float Boid::max_speed = 3.0f;
float Boid::max_force = 0.1f;
float Boid::separation_weight = 0.5f;
float Boid::alignment_weight = 0.3f;
float Boid::cohesion_weight = 0.5f;

Boid::Boid(glm::vec2 init_position, glm::vec2 init_velocity)
    : position{init_position}, velocity{init_velocity},
      acceleration{0.0f, 0.0f} {}

void Boid::update_position(float frame_duration) {
  velocity += acceleration;
  velocity = glm::length(velocity) > max_speed
                 ? glm::normalize(velocity) * max_speed
                 : velocity;
  position += velocity * frame_duration;
  acceleration = glm::vec2(0.0f);
}

void Boid::apply_bounds(const glm::vec2 &top_left,
                        const glm::vec2 &bottom_right) {
  if (position.x < top_left.x)
    position.x = bottom_right.x - (top_left.x - position.x);
  else if (position.x > bottom_right.x)
    position.x = top_left.x + (position.x - bottom_right.x);
  if (position.y < top_left.y)
    position.y = bottom_right.y - (top_left.y - position.y);
  else if (position.y > bottom_right.y)
    position.y = top_left.y + (position.y - bottom_right.y);
}

glm::vec2 Boid::separation(const std::vector<Boid> &boids) {
  glm::vec2 steer = {0.0f, 0.0f};
  int count = 0;
  for (const auto &other : boids) {
    glm::vec2 diff = position - other.position;
    float d = glm::length2(diff);
    if (&other != this && d < radius * radius) {
      diff = glm::normalize(diff) / d;
      steer += diff;
      count++;
    }
  }
  if (count > 0) {
    steer /= static_cast<float>(count);
    steer = glm::length(steer) > max_speed ? glm::normalize(steer) * max_speed
                                           : steer;
    glm::vec2 force = steer - velocity;
    force = glm::length(force) > max_force ? glm::normalize(force) * max_force
                                           : force;
    return force * separation_weight;
  }
  return glm::vec2(0.0f);
}

glm::vec2 Boid::alignment(const std::vector<Boid> &boids) {
  glm::vec2 avg_vel = {0.0f, 0.0f};
  int count = 0;
  for (const auto &other : boids) {
    if (&other != this &&
        glm::length2(position - other.position) < radius * radius) {
      avg_vel += other.velocity;
      count++;
    }
  }
  if (count > 0) {
    avg_vel /= static_cast<float>(count);
    avg_vel = glm::length(avg_vel) > max_speed
                  ? glm::normalize(avg_vel) * max_speed
                  : avg_vel;
    glm::vec2 force = avg_vel - velocity;
    force = glm::length(force) > max_force ? glm::normalize(force) * max_force
                                           : force;
    return force * alignment_weight;
  }
  return glm::vec2(0.0f);
}

glm::vec2 Boid::cohesion(const std::vector<Boid> &boids) {
  glm::vec2 center_mass = {0.0f, 0.0f};
  int count = 0;
  for (const auto &other : boids) {
    if (&other != this &&
        glm::length2(position - other.position) < radius * radius) {
      center_mass += other.position;
      count++;
    }
  }
  if (count > 0) {
    center_mass /= static_cast<float>(count);
    glm::vec2 steer = center_mass - position;
    steer = glm::length(steer) > max_speed ? glm::normalize(steer) * max_speed
                                           : steer;
    glm::vec2 force = steer - velocity;
    force = glm::length(force) > max_force ? glm::normalize(force) * max_force
                                           : force;
    return force * cohesion_weight;
  }
  return glm::vec2(0.0f);
}

void Boid::update_acceleration(const std::vector<Boid> &boids) {
  glm::vec2 sep = separation(boids);
  glm::vec2 ali = alignment(boids);
  glm::vec2 coh = cohesion(boids);
  acceleration += sep + ali + coh;
}

BoidsSystem::BoidsSystem(int num_boids, glm::vec2 top_left,
                         glm::vec2 bottom_right)
    : top_left_bound{top_left}, bottom_right_bound{bottom_right} {
  for (int i = 0; i < num_boids; ++i) {
    glm::vec2 pos = glm::linearRand(top_left, bottom_right);
    glm::vec2 vel = p6::random::direction() * Boid::max_speed;
    boids.emplace_back(pos, vel);
  }
}

void BoidsSystem::update(float frame_duration) {
  for (auto &boid : boids) {
    boid.update_acceleration(boids);
    boid.update_position(frame_duration);
    boid.apply_bounds(top_left_bound, bottom_right_bound);
  }
}

void BoidsSystem::draw(p6::Context &ctx) const {
  for (const auto &boid : boids) {
    ctx.circle(boid.position, 0.02f);
  }
}

void BoidsSystem::resize(int new_size, glm::vec2 top_left,
                         glm::vec2 bottom_right) {
  boids.clear();
  boids.reserve(new_size);
  for (int i = 0; i < new_size; ++i) {
    glm::vec2 pos = glm::linearRand(top_left, bottom_right);
    glm::vec2 vel = p6::random::direction() * Boid::max_speed;
    boids.emplace_back(pos, vel);
  }
}