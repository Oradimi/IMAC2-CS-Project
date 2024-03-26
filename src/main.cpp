#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cstdlib>
#include <ctime>
#include <imgui.h>
#define DOCTEST_CONFIG_IMPLEMENT
#include "boid.hpp"
#include "doctest/doctest.h"
#include "glimac/cube_vertices.hpp"
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
  std::vector<Boid> swarm(20);

  Renderer renderer;
  RenderedObject boid_mesh{glimac::cone_vertices(2.f, 1.2f, 6.f, 6.f)};
  RenderedObject cube_mesh{glimac::cube_vertices(1.0f, 1.0f, 1.0f)};
  renderer.ctx.update = [&]() {
    renderer.clearAll();

    // ImGui::Begin("Parameters");
    // Boid::initializeUIElements();
    // ImGui::End();
    renderer.drawObject(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), cube_mesh);

    for (Boid &boid : swarm) {
      boid.move();
      boid.separation(swarm);
      boid.alignment(swarm);
      boid.cohesion(swarm);
      renderer.drawObject(boid.getPosition(), boid.getVelocity(), boid_mesh);
    }
  };

  renderer.ctx.imgui = [&]() {
    ImGui::Begin("Parameters");
    Boid::initializeUIElements();
    ImGui::End();
  };

  renderer.handleLookAround();
  renderer.handleZoom();

  renderer.start();

  renderer.close();

  return 0;
}