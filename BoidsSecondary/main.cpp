#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include "RandomUtils.hpp"
#include "boids.hpp"
#include "doctest/doctest.h"
#include <p6/p6.h>

int main() {
  if (doctest::Context{}.run() != 0) {
    return EXIT_FAILURE;
  }

  auto ctx = p6::Context{{.title = "Boids Simulation"}};
  ctx.maximize_window();

  glm::vec2 square_top_left{-0.5f, -0.5f};
  glm::vec2 square_bottom_right{0.5f, 0.5f};
  BoidsSystem boids_system(100, square_top_left, square_bottom_right);
  int new_num_boids = 100;

  const float default_max_speed = 3.0f;
  const float default_separation_weight = 0.5f;
  const float default_alignment_weight = 0.3f;
  const float default_cohesion_weight = 0.5f;

  float min_speed = 1.0f, max_speed = 5.0f;
  float bernoulli_p = 0.5f;
  int n_trials = 10;
  float binomial_p = 0.5f;
  float lambda = 0.1f;
  float mu = 0.5f, b = 0.15f;
  int n_IrwinHall = 12;

  ctx.update = [&]() {
    ctx.background(p6::NamedColor::Blue);
    ctx.square(p6::Center{0.f, 0.f}, p6::Radius{0.5f},
               p6::Rotation{p6::Radians{0.f}});

    boids_system.update(ctx.delta_time());
    boids_system.draw(ctx);

    ImGui::Begin("Boids Settings");
    ImGui::SliderFloat("Separation Weight", &Boid::separation_weight, 0.0f,
                       1.0f, "Weight: %.3f");
    ImGui::SliderFloat("Alignment Weight", &Boid::alignment_weight, 0.0f, 1.0f,
                       "Weight: %.3f");
    ImGui::SliderFloat("Cohesion Weight", &Boid::cohesion_weight, 0.0f, 1.0f,
                       "Weight: %.3f");
    ImGui::SliderFloat("Max Speed", &Boid::max_speed, 0.0f, 10.0f,
                       "Speed: %.3f");

    ImGui::SliderFloat("Min Speed", &min_speed, 0.0f, 1.0f, "Min: %.2f");
    ImGui::SliderFloat("Max Speed Distribution", &max_speed, 1.0f, 10.0f,
                       "Max: %.2f");
    if (ImGui::Button("Apply Uniform Distribution to Max Speed")) {
      Boid::max_speed = RandomUtils::generateUniform(min_speed, max_speed);
    }

    ImGui::SliderFloat("Probability for Bernoulli", &bernoulli_p, 0.0f, 1.0f,
                       "p: %.2f");
    if (ImGui::Button("Apply Bernoulli Distribution")) {
    }

    ImGui::SliderInt("Number of Trials for Binomial", &n_trials, 1, 20);
    ImGui::SliderFloat("Probability for Binomial", &binomial_p, 0.0f, 1.0f,
                       "p: %.2f");
    if (ImGui::Button("Apply Binomial Distribution")) {
    }

    ImGui::SliderFloat("Lambda for Exponential", &lambda, 0.01f, 1.0f,
                       "Lambda: %.3f");
    ImGui::SliderFloat("Mu for Laplace", &mu, 0.0f, 1.0f, "Mu: %.3f");
    ImGui::SliderFloat("B for Laplace", &b, 0.01f, 0.3f, "B: %.3f");
    ImGui::SliderInt("N for Irwin Hall", &n_IrwinHall, 1, 24);
    if (ImGui::Button("Apply Irwin Hall to Alignment")) {
      Boid::alignment_weight =
          RandomUtils::generateIrwinHall(n_IrwinHall) / n_IrwinHall;
    }

    ImGui::InputInt("Number of Boids", &new_num_boids);
    if (ImGui::Button("Apply Number of Boids")) {
      boids_system.resize(std::max(1, new_num_boids), square_top_left,
                          square_bottom_right);
    }

    if (ImGui::Button("Reset")) {
      Boid::max_speed = default_max_speed;
      Boid::separation_weight = default_separation_weight;
      Boid::alignment_weight = default_alignment_weight;
      Boid::cohesion_weight = default_cohesion_weight;

      min_speed = 1.0f;
      max_speed = 5.0f;
      bernoulli_p = 0.5f;
      n_trials = 10;
      binomial_p = 0.5f;
      lambda = 0.1f;
      mu = 0.5f;
      b = 0.15f;
      n_IrwinHall = 12;
    }

    ImGui::End();
  };

  ctx.start();
  return 0;
}