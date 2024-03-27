#pragma once

#include <limits>

namespace ConstexprMath {

template <class T> T constexpr sqrtNewtonRaphson(T x, T curr, T prev) {
  return curr == prev ? curr
                      : sqrtNewtonRaphson<T>(x, 0.5 * (curr + x / curr), curr);
}

/** Constexpr version of the square root
 * Return value:
 *   - For a finite and non-negative value of "x", returns an approximation for
 * the square root of "x"
 *   - Otherwise, returns NaN
 */
template <class T> T constexpr sqrt(T x) {
  return x >= 0 && x < std::numeric_limits<T>::infinity()
             ? ConstexprMath::sqrtNewtonRaphson<T>(x, x, 0)
             : std::numeric_limits<T>::quiet_NaN();
}

} // namespace ConstexprMath
