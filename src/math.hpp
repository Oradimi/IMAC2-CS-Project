#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <limits>
#include <random>

class RandomMath {
private:
  std::mt19937 generator = getRandomGenerator();

public:
  static inline std::mt19937 getRandomGenerator() {
    return std::mt19937(std::random_device{}());
  }

  /// Base random function. Returns a number strictly between 0 and 1
  inline double generateRandom() {
    return std::generate_canonical<double, std::numeric_limits<double>::digits>(
        generator);
  }

  /// Uniform distribution. Returns a number strictly between min and max
  inline double generateUniform(double min, double max) {
    double x = generateRandom();

    return x * (max - min) + min;
  }

  /// Bernoulli distribution.
  /// Returns true if the random number is strictly below p
  /// p represents the probability of success of an event
  inline bool generateBernoulli(double p) {
    double x = generateRandom();

    return x < p;
  }

  inline double generateExponential(double lambda) {
    double x = generateRandom();

    return -std::log(x) / lambda;
  }

  inline double expectedValueExponential(double lambda) {
    double sum = 0;
    for (int i = 0; i < 10; ++i) {
      double rand_num = generateExponential(lambda);
      sum += rand_num;
    }
    return sum;
  }

  // glm::vec2 randomGradient(std::mt19937 &generator) {
  //   auto u = static_cast<float>(generateUniform(generator));

  //   glm::vec2 v;
  //   v.x = sinf(u);
  //   v.y = cosf(u);

  //   return v;
  // }

  // // Computes the dot product of the distance and gradient vectors.
  // float dotGridGradient(int ix, int iy, float x, float y) {
  //   // Get gradient from integer coordinates
  //   glm::vec2 gradient; /* = randomGradient(generator, ix, iy);*/

  //   // Compute the distance vector
  //   float dx = x - (float)ix;
  //   float dy = y - (float)iy;

  //   // Compute the dot-product
  //   return (dx * gradient.x + dy * gradient.y);
  // }

  // float interpolate(float a0, float a1, float w) {
  //   return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
  // }

  // // Sample Perlin noise at coordinates x, y
  // float perlin(float x, float y) {

  //   // Determine grid cell corner coordinates
  //   int x0 = static_cast<int>(x);
  //   int y0 = static_cast<int>(y);
  //   int x1 = x0 + 1;
  //   int y1 = y0 + 1;

  //   // Compute Interpolation weights
  //   float sx = x - static_cast<float>(x0);
  //   float sy = y - static_cast<float>(y0);

  //   // Compute and interpolate top two corners
  //   float n0 = dotGridGradient(x0, y0, x, y);
  //   float n1 = dotGridGradient(x1, y0, x, y);
  //   float ix0 = interpolate(n0, n1, sx);

  //   // Compute and interpolate bottom two corners
  //   n0 = dotGridGradient(x0, y1, x, y);
  //   n1 = dotGridGradient(x1, y1, x, y);
  //   float ix1 = interpolate(n0, n1, sx);

  //   // Final step: interpolate between the two previously interpolated
  //   values, now
  //   // in y
  //   float value = interpolate(ix0, ix1, sy);

  //   return value;
  // }

  // int main() {
  //   std::srand(rdtsc());

  //   const double lambda = 0.5;
  //   int num_samples = 10000;

  //   double sum = 0.0;
  //   for (int i = 0; i < num_samples; ++i) {
  //     sum += generateExponential(lambda);
  //   }

  //   double average = sum / num_samples;

  //   double expected_value = 1.0 / lambda;

  //   return 0;
  // }

  // // pile ou face
  // float p = 0.5;
  // float random_number_2 = std::rand() / (float)RAND_MAX - p;
  // std::cout << random_number_2 << std::endl;
};

namespace ConstexprMath {

template <typename T> T constexpr sqrtNewtonRaphson(T x, T curr, T prev) {
  return curr == prev ? curr
                      : sqrtNewtonRaphson<T>(
                            x, static_cast<T>(0.5) * (curr + x / curr), curr);
}

/** Constexpr version of the square root
 * Return value:
 *   - For a finite and non-negative value of "x", returns an approximation
 * for the square root of "x"
 *   - Otherwise, returns NaN
 */
template <typename T> T constexpr sqrt(T x) {
  return x >= static_cast<T>(0) && x < std::numeric_limits<T>::infinity()
             ? sqrtNewtonRaphson<T>(x, x, static_cast<T>(0))
             : std::numeric_limits<T>::quiet_NaN();
}

} // namespace ConstexprMath
