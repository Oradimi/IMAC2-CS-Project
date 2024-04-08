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

  std::mt19937 generator(RandomMath::getRandomGenerator());

  double random_number = RandomMath::generateUniform(generator);

  std::vector<Boid> swarm;
  swarm.reserve(20);

  for (int i = 0; i < 20; ++i) {
    swarm.emplace_back(generator);
  }

  Renderer renderer;

  RenderedObject boidMesh{loadOBJ("meina.obj"), "MeinaDiffuse.png",
                          "3D.vs.glsl", "tex3D.fs.glsl"};

  RenderedObject cubeMesh{loadOBJ("insideOutCube.obj"), "White.png",
                          "3D.vs.glsl", "tex3D.fs.glsl"};

  renderer.ctx.update = [&]() {
    renderer.clearAll();

    glm::mat4 cubeModelMatrix =
        glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 0.f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds()});
    renderer.drawObject(cubeModelMatrix, cubeMesh);

    for (Boid &boid : swarm) {
      boid.move(swarm);
      glm::mat4 boidModelMatrix =
          glm::translate(glm::mat4{1.f}, boid.getPosition()) *
          computeRotationMatrix(boid.getVelocity());
      renderer.drawObject(boidModelMatrix, boidMesh);
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