#pragma once

#include "glm/fwd.hpp"
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

enum MarkovState { LOW, HIGH, FLOOD };

class RandomMath {
private:
  std::mt19937 generator = getRandomGenerator();

public:
  static inline std::mt19937 getRandomGenerator() {
    return std::mt19937(std::random_device{}());
  }

  /// Base random function
  /// Returns a number strictly between 0 and 1 in uniform distribution
  inline double generateRandom() {
    return std::generate_canonical<double, std::numeric_limits<double>::digits>(
        generator);
  }

  ///* DISTRIBUTION FUNCTIONS *///

  /// Returns a number strictly between min and max
  inline double generateUniform(double min, double max) {
    double x = generateRandom();

    return x * (max - min) + min;
  }

  /// Returns an integer between min included and max excluded
  inline glm::int32 generateUniformDiscrete(int min, int max) {
    double x = generateRandom();

    return static_cast<glm::int32>(floor(x * (max - min) + min));
  }

  /// Returns true if the random number is strictly below p
  /// p represents the probability of success of an event
  inline bool generateBernoulli(double p) {
    double x = generateRandom();

    return x < p;
  }

  inline int generateBinomial(double p, int n) {
    int x = 0; // number of successes
    for (int i = 0; i < n; i++) {
      generateBernoulli(p) && x++;
    }
    return x;
  }

  /// lambda represents the frequency of an event
  inline double generateExponential(double lambda) {
    double x = generateRandom();

    return -std::log(1.0 - x) / lambda;
  }

  /// Laplace distibution with position mu and scale b
  inline double generateLaplace(double mu, double b) {
    double x = generateRandom();

    if (x < 0.5)
      return mu + b * std::log(2.0 * x);
    return mu - b * std::log(2.0 - 2.0 * x);
  }

  /// Irwin-Hall distribution
  /// n represents the number of iterations, mu is the position
  /// defaut values approximate a normal distribution between -3 and 3
  inline double generateIrwinHall(int n = 12, double mu = -6.0) {
    double x = 0.0;
    for (int i = 0; i < n; i++) {
      x += generateRandom();
    }
    return x + mu;
  }

  inline double generateBeta(double alpha, double beta, int n = 2000) {
    double x = 0.0;

    for (int i = 0; i < n; i++) {
      x = generateRandom();
      if (x < beta / (beta + alpha))
        beta += 1.0;
      else
        alpha += 1.0;
    }

    return alpha / (beta + alpha);
  }

  ///* MARKOV CHAIN *///

  inline MarkovState getNextMarkovState(const glm::mat3 &markovMatrix,
                                        int currentState) {
    double x = generateRandom();

    double cumulativeProbability = 0.0;
    for (int nextState = 0; nextState < 3; ++nextState) {
      cumulativeProbability += markovMatrix[currentState][nextState];
      if (x < cumulativeProbability) {
        return static_cast<MarkovState>(nextState);
      }
    }

    // Default return (should never reach here)
    return static_cast<MarkovState>(currentState);
  }
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
