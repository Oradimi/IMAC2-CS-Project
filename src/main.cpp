#define DOCTEST_CONFIG_IMPLEMENT

#include "boid.hpp"
#include "doctest/doctest.h"
#include "glimac/cube_vertices.hpp"
#include "glimac/math.hpp"
#include "glm/fwd.hpp"
#include "loader/objLoader.hpp"
#include "renderer.hpp"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <vector>

int main() {
  // Run the tests
  if (doctest::Context{}.run() != 0)
    return EXIT_FAILURE;

  std::mt19937 generator = RandomMath::getRandomGenerator();

  double random_number = RandomMath::generateUniform(generator);

  std::vector<Boid> swarm(20 /*, Boid{generator}*/);

  Renderer renderer;

  RenderedObject boid_mesh{loadOBJ("meina.obj"), "MeinaDiffuse.png",
                           "3D.vs.glsl", "tex3D.fs.glsl"};

  // RenderedObject cube_mesh{loadOBJ("cube.obj"), "MeinaDiffuse.png",
  //                          "3D.vs.glsl", "tex3D.fs.glsl"};

  renderer.ctx.update = [&]() {
    renderer.clearAll();

    // renderer.drawObject(glm::vec3{0.f}, glm::vec3{0.f}, cube_mesh);

    for (Boid &boid : swarm) {
      boid.move(swarm);
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