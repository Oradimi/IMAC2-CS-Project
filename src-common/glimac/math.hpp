#pragma once

#include "glm/fwd.hpp"
#include <cmath>
#include <cstdlib>
#include <limits>
#include <random>

namespace RandomMath {

inline std::mt19937 getRandomGenerator() {
  return std::mt19937(std::random_device{}());
}

inline double generateUniform(std::mt19937 &generator) {
  return std::generate_canonical<double, std::numeric_limits<double>::digits>(
      generator);
}

inline double generateExponential(std::mt19937 &generator, double lambda) {
  double u = generateUniform(generator);

  // loi exponentielle
  double x = -std::log(u) / lambda;

  return x;
}

inline double expectedValueExponential(std::mt19937 &generator, double lambda) {
  double sum = 0;
  for (int i = 0; i < 10; ++i) {
    double rand_num = generateExponential(generator, lambda);
    sum += rand_num;
  }
  return 0.0;
}

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

} // namespace RandomMath

namespace ConstexprMath {

template <typename T> T constexpr sqrtNewtonRaphson(T x, T curr, T prev) {
  return curr == prev ? curr
                      : sqrtNewtonRaphson<T>(
                            x, static_cast<T>(0.5) * (curr + x / curr), curr);
}

/** Constexpr version of the square root
 * Return value:
 *   - For a finite and non-negative value of "x", returns an approximation for
 * the square root of "x"
 *   - Otherwise, returns NaN
 */
template <typename T> T constexpr sqrt(T x) {
  return x >= static_cast<T>(0) && x < std::numeric_limits<T>::infinity()
             ? sqrtNewtonRaphson<T>(x, x, static_cast<T>(0))
             : std::numeric_limits<T>::quiet_NaN();
}

} // namespace ConstexprMath
