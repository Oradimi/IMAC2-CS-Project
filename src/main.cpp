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
  // RenderedObject boid_mesh;

  renderer.addObject(RenderedObject{glimac::cone_vertices(2.f, 2.f, 4.f, 4.f)});

  renderer.ctx.update = [&]() {
    renderer.clearAll();
    // for (Boid boid : swarm) {
    //   boid.dothing();
    //   renderer.drawObject(boid.positionStuff, boid_mesh);
    // }
    renderer.drawObject();
  };

  renderer.handleLookAround();
  renderer.handleZoom();

  renderer.start();

  renderer.close();

  return 0;
}
