#include <cstdlib>
#include <ctime>
#define DOCTEST_CONFIG_IMPLEMENT
#include "boid.hpp"
#include "doctest/doctest.h"
// #include "renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

int main() {
  // Run the tests
  if (doctest::Context{}.run() != 0)
    return EXIT_FAILURE;

  // const img::Image imageEarth =
  //     p6::load_image_buffer("assets/textures/EarthMap.jpg");
  // const img::Image imageMoon =
  //     p6::load_image_buffer("assets/textures/MoonMap.jpg");
  // const img::Image imageCloud =
  //     p6::load_image_buffer("assets/textures/CloudMap.jpg");

  // // Actual application code
  auto ctx = p6::Context{{.title = "IMAC2-CS-Project"}};
  ctx.maximize_window();

  std::srand(std::time(nullptr));
  std::vector<Boid> swarm;
  swarm.reserve(5);
  for (int i = 0; i < 5; i++) {
    swarm.emplace_back();
  }

  // Renderer r;

  // Declare your infinite update loop.
  ctx.update = [&]() {
    ctx.background(p6::NamedColor::Blue);
    for (Boid &boid : swarm) {
      // boid.avoid(swarm);
      // Dessiner un triangle à la place d'un cercle
      glm::vec2 pos = boid.move();
      float size = boid.getRadius();
      glm::vec2 p1{size, 0.0f};
      glm::vec2 p2{-size, size};
      glm::vec2 p3{-size, -size};
      ctx.triangle(p1, p2, p3, pos, boid.getAngle());
    }
  };

  // r.defineTextures();

  // r.addTexture("EarthMap.jpg");
  // r.addTexture("CloudMap.jpg");
  // r.addTexture("MoonMap.jpg");

  // r.ctx.update = [&]() {
  //   r.clearAll();
  //   // Earth
  //   r.drawEarth();

  //   // Moons
  //   std::cout << "welsh4" << std::endl;
  //   r.drawMoon();
  // };

  // // Should be done last. It starts the infinite loop.
  // r.start();

  ctx.start();

  return 0;
}
