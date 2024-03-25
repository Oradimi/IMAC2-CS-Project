#pragma once

#include "boid.hpp"
#include "glm/fwd.hpp"
#include <cstdlib>
#include <imgui.h>
#include <p6/p6.h>
#include <vector>

float Boid::turn_factor = 2.f;
float Boid::vision_range = 1.f;
float Boid::protected_range = 0.5f;
float Boid::centering_factor = 0.000005f;
float Boid::avoid_factor = 0.0005f;
float Boid::matching_factor = 0.0005f;
float Boid::max_speed = 0.02f;
float Boid::min_speed = 0.002f;
float Boid::max_bias = 0.01f;
float Boid::bias_increment = 0.00004f;

glm::vec3 clampVectorSpeed(glm::vec3 vec, float max) {
  // std::cout << max / glm::length(vec) << std::endl;
  if (glm::length(vec) > max)
    return {vec.x * max / glm::length(vec), vec.y * max / glm::length(vec),
            vec.z * max / glm::length(vec)};
  return vec;
}

Boid::Boid() {
  pos = {static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f),
         static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f),
         static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f)};

  vel = {static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f),
         static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f),
         static_cast<float>(std::rand() - RAND_MAX * 0.5f) / (RAND_MAX * 0.5f)};
}

void Boid::initializeUIElements() {
  ImGui::DragFloat("Turn Factor", &turn_factor);
  ImGui::DragFloat("Vision Range", &vision_range);
  ImGui::DragFloat("Protected Range", &protected_range);
  ImGui::DragFloat("Centering Factor", &centering_factor);
  ImGui::DragFloat("Avoid Factor", &avoid_factor);
  ImGui::DragFloat("Matching Factor", &matching_factor);
  ImGui::DragFloat("Max Speed", &max_speed);
  // ImGui::DragFloat("Min Speed", &min_speed);
  // ImGui::DragFloat("Max Bias", &max_bias);
  // ImGui::DragFloat("Bias Increment", &bias_increment);
}

void Boid::move() {
  float bounds = 2.f;
  pos += vel;
  vel *= 0.9999;
  vel = clampVectorSpeed(vel, max_speed);

  for (int i = 0; i < 3; i++) {
    if (pos[i] > bounds) {
      vel[i] = vel[i] - turn_factor;
    } else if (pos[i] < -bounds) {
      vel[i] = vel[i] + turn_factor;
    }
  }
}

void Boid::separation(const std::vector<Boid> &swarm) {
  glm::vec3 avoid_dir{0.0f};

  for (const Boid &boid : swarm) {
    if (&boid == this)
      continue;

    float distance = glm::distance(pos, boid.pos);

    if (distance < protected_range) {
      avoid_dir += pos - boid.pos;
    }
  }

  vel += avoid_dir * avoid_factor;
}

void Boid::alignment(const std::vector<Boid> &swarm) {
  glm::vec3 vel_average{0.0f};
  float neighboring_boids = 0.f;

  for (const Boid &boid : swarm) {
    if (&boid == this)
      continue;

    float distance = glm::distance(pos, boid.pos);

    if (distance < vision_range) {
      vel_average += boid.pos;
      neighboring_boids += 1.f;
    }
  }

  if (neighboring_boids > 0) {
    vel_average = vel_average / neighboring_boids;
  }

  vel += (vel_average - vel) * matching_factor;
}

void Boid::cohesion(const std::vector<Boid> &swarm) {
  glm::vec3 pos_average{0.0f};
  float neighboring_boids = 0.f;

  for (const Boid &boid : swarm) {
    if (&boid == this)
      continue;

    float distance = glm::distance(pos, boid.pos);

    if (distance < vision_range) {
      pos_average += boid.pos;
      neighboring_boids += 1.f;
    }
  }

  if (neighboring_boids > 0) {
    pos_average = pos_average / neighboring_boids;
  }

  vel += (pos_average - pos) * centering_factor;
}
