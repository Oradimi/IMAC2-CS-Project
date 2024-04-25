#pragma once

#include <cmath>
#include <p6/p6.h>


class RandomUtils {
public:
  static float generateUniform(float min, float max) {
    return min + (max - min) * p6::random::number();
  }

  static bool generateBernoulli(float p) { return p6::random::number() < p; }

  static int generateBinomial(int n, float p) {
    int x = 0;
    for (int i = 0; i < n; i++) {
      if (p6::random::number() < p) {
        x++;
      }
    }
    return x;
  }

  static float generateExponential(float lambda) {
    return -std::log(1.0f - p6::random::number()) / lambda;
  }

  static float generateLaplace(float mu, float b) {
    float u = p6::random::number();
    return u < 0.5f ? mu + b * std::log(2.0f * u)
                    : mu - b * std::log(2.0f - 2.0f * u);
  }

  static float generateIrwinHall(int n = 12) {
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
      sum += p6::random::number();
    }
    return sum - 6.0f; // Adjusted to center around 0
  }

  static glm::vec2 generateRandomDirection() { return p6::random::direction(); }

  static glm::vec2 generateRandomPoint(float aspect_ratio = 1.0f) {
    return glm::vec2(p6::random::number(-aspect_ratio, aspect_ratio),
                     p6::random::number(-1.0f, 1.0f));
  }
};
