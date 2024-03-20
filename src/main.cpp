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

  Renderer r;

  r.addTexture("EarthMap.jpg");
  r.addTexture("CloudMap.jpg");
  r.addTexture("MoonMap.jpg");

  r.defineTextures();

  std::vector<glm::vec3> randomAxes = createRandomlySpreadSpheres();

  r.ctx.update = [&]() {
    r.clearAll();
    // Earth
    r.drawEarth();

    // Moons
    r.drawMoon(randomAxes);
  };

  r.handleLookAround();
  r.handleZoom();

  r.start();

  r.close();

  return 0;
}
