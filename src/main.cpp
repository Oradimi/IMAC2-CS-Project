#include <cstdlib>
#include <ctime>
#define DOCTEST_CONFIG_IMPLEMENT
#include "boid.hpp"
#include "doctest/doctest.h"
#include "renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

int main() {
  // Run the tests
  if (doctest::Context{}.run() != 0)
    return EXIT_FAILURE;

  std::srand(std::time(nullptr));
  std::vector<Boid> swarm;
  swarm.reserve(5);
  for (int i = 0; i < 5; i++) {
    swarm.emplace_back();
  }

  Renderer renderer;
  RenderedObject boid_mesh{glimac::cone_vertices(2.f, 1.2f, 6.f, 6.f)};

  renderer.ctx.update = [&]() {
    renderer.clearAll();
    for (Boid &boid : swarm) {
      boid.move();
      renderer.drawObject(boid.getPosition(), boid.getVelocity(), boid_mesh);
    }
  };

  renderer.handleLookAround();
  renderer.handleZoom();

  renderer.start();

  renderer.close();

  return 0;
}
