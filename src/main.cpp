#include <cstdlib>
#include <ctime>
#define DOCTEST_CONFIG_IMPLEMENT
#include "boid.hpp"
#include "doctest/doctest.h"

int main() {
  // Run the tests
  if (doctest::Context{}.run() != 0)
    return EXIT_FAILURE;

  // Actual application code
  auto ctx = p6::Context{{.title = "IMAC2-CS-Project"}};
  ctx.maximize_window();

  std::srand(std::time(nullptr));
  std::vector<Boid> swarm;
  swarm.reserve(5);
  for (int i = 0; i < 5; i++) {
    swarm.emplace_back();
  }

  // Declare your infinite update loop.
  ctx.update = [&]() {
    ctx.background(p6::NamedColor::Blue);
    for (Boid &boid : swarm) {
      boid.avoid(swarm);
      ctx.circle(p6::Center{boid.move()}, p6::Radius{boid.getRadius()});
    }
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
}
