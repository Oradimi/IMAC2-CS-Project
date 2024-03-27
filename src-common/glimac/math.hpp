#pragma once

#include <limits>

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
