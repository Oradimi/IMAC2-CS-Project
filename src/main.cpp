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

  std::vector<glm::vec3> randomAxes = createRandomlySpreadSpheres();

  EarthProgram earthProgram{};
  MoonProgram moonProgram{};

  r.ctx.update = [&]() {
    r.clearAll();
    r.drawEarth(earthProgram);
    r.drawMoon(moonProgram, randomAxes);
  };

  r.handleLookAround();
  r.handleZoom();

  r.start();

  earthProgram.earthObject.close();
  moonProgram.moonObject.close();

  return 0;
}
