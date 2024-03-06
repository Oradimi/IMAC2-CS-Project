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
      // Dessiner un triangle à la place d'un cercle
      glm::vec2 pos = boid.move();
      float size = boid.getRadius();
      glm::vec2 p1 = pos + glm::vec2(size, 0.0f);
      glm::vec2 p2 = pos + glm::vec2(-size, size);
      glm::vec2 p3 = pos + glm::vec2(-size, -size);
      ctx.triangle({p1.x, p1.y}, {p2.x, p2.y}, {p3.x, p3.y});
    }
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();

  return 0;
}