#include "doctest/doctest.h"
#include "math.hpp"

// This is just an example of how to use Doctest in order to write tests.
// To learn more about Doctest, see
// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
TEST_CASE("generateRandom returns a number between 0 and 1") {
  RandomMath rand;
  CHECK(rand.generateRandom() > 0);
  CHECK(rand.generateRandom() < 1);
}
