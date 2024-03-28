#pragma once

#include "boid.hpp"
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <cstdlib>
#include <imgui.h>
#include <p6/p6.h>
#include <vector>

float Boid::bounds = 80.f;
float Boid::turn_factor = 0.2f;
float Boid::protected_range = 4.0f;
float Boid::vision_range = 10.0f;
float Boid::avoid_factor = 0.05f;
float Boid::match_factor = 0.3f;
float Boid::center_factor = 0.001f;
float Boid::min_speed = 0.4f;
float Boid::max_speed = 2.0f;
float Boid::max_bias = 0.01f;
float Boid::bias_increment = 0.00004f;

void clampVectorMagnitude(glm::vec3 &vec, float min, float max) {
  float speed = glm::length(vec);
  if (speed < min)
    vec = (vec / speed) * min;
  if (speed > max)
    vec = (vec / speed) * max;
}

Boid::Boid() {

  pos =
      glm::vec3{
          static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f),
          static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f),
          static_cast<float>(std::rand() - RAND_MAX * 0.5f) /
              (RAND_MAX * 0.5f)} *
      bounds;

  vel =
      glm::vec3{
          static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f),
          static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f),
          static_cast<float>(std::rand() - RAND_MAX * 0.5f) /
              (RAND_MAX * 0.5f)} *
          (max_speed - min_speed) +
      min_speed;
}

void Boid::initializeUIElements() {
  ImGui::SliderFloat("Bounds", &bounds, 2.f, 100.f);
  ImGui::SliderFloat("Turn", &turn_factor, 0.04f, 1.0f);
  ImGui::SliderFloat("Protected Range", &protected_range, 0.4f,
                     vision_range - 0.1f);
  ImGui::SliderFloat("Vision Range", &vision_range, protected_range + 0.1f,
                     100.f);
  ImGui::SliderFloat("Avoid", &avoid_factor, 0.f, 1.f);
  ImGui::SliderFloat("Match", &match_factor, 0.f, 1.f);
  ImGui::SliderFloat("Center", &center_factor, 0.f, 0.01f);
  ImGui::SliderFloat("Min Speed", &min_speed, 0.01f, max_speed - 0.1f);
  ImGui::SliderFloat("Max Speed", &max_speed, min_speed + 0.1f, 10.f);
  // ImGui::SliderFloat("Max Bias", &max_bias);
  // ImGui::SliderFloat("Bias Increment", &bias_increment);
}

void Boid::move(const std::vector<Boid> &swarm) {
  flock(swarm);

  avoidBounds();

  clampVectorMagnitude(vel, min_speed, max_speed);

  pos += vel;
}

void Boid::flock(const std::vector<Boid> &swarm) {
  glm::vec3 avoid_dir{0.0f};
  glm::vec3 pos_average{0.0f};
  glm::vec3 vel_average{0.0f};
  float neighboring_boids = 0.f; // excluding ones that are too close

  for (const Boid &otherBoid : swarm) {
    if (&otherBoid == this)
      continue;

    float distance = glm::distance(pos, otherBoid.pos);

    if (distance < protected_range) {
      avoid_dir += pos - otherBoid.pos;
    } else if (distance < vision_range) {
      pos_average += otherBoid.pos;
      vel_average += otherBoid.vel;
      neighboring_boids++;
    }
  }

  if (neighboring_boids > 0) {
    pos_average = pos_average / neighboring_boids;
    vel_average = vel_average / neighboring_boids;

    vel = vel + (pos_average - pos) * center_factor +
          (vel_average - vel) * match_factor;
  }

  vel = vel + avoid_dir * avoid_factor;
}

void Boid::avoidBounds() {
  for (int i = 0; i < 3; i++) {
    if (pos[i] > bounds) {
      vel[i] = vel[i] - turn_factor;
    } else if (pos[i] < -bounds) {
      vel[i] = vel[i] + turn_factor;
    }
  }
}
