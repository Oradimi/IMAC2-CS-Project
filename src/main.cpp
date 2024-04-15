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

  RenderedObject boidMesh{loadOBJ("spaceship.obj"), "spaceship.png",
                          "3D.vs.glsl", "tex3D.fs.glsl"};

  RenderedObject cubeMesh{loadOBJ("insideOutCube.obj"), "Gray.png",
                          "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject treeMesh{loadOBJ("tree.obj"), "Gray.png", "3D.vs.glsl",
                          "directionalLight.fs.glsl"};

  RenderedObject firehydrantMesh{loadOBJ("firehydrant.obj"), "Gray.png",
                                 "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject trafficconeMesh{loadOBJ("trafficcone.obj"), "Gray.png",
                                 "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject trashbagsMesh{loadOBJ("trashbags.obj"), "Gray.png",
                               "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject stopsignMesh{loadOBJ("stopsign.obj"), "Gray.png", "3D.vs.glsl",
                              "directionalLight.fs.glsl"};

  RenderedObject carMesh{loadOBJ("car.obj"), "Gray.png", "3D.vs.glsl",
                         "directionalLight.fs.glsl"};

  RenderedObject trafficlightMesh{loadOBJ("trafficlight.obj"), "Gray.png",
                                  "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject benchMesh{loadOBJ("bench.obj"), "Gray.png", "3D.vs.glsl",
                           "directionalLight.fs.glsl"};

  RenderedObject mailboxMesh{loadOBJ("Mailbox.obj"), "Gray.png", "3D.vs.glsl",
                             "directionalLight.fs.glsl"};

  RenderedObject alienMesh{loadOBJ("alien.obj"), "Gray.png", "3D.vs.glsl",
                           "directionalLight.fs.glsl"};

  renderer.ctx.update = [&]() {
    renderer.clearAll();

    glEnable(GL_CULL_FACE);
    glm::mat4 cubeModelMatrix =
        glm::translate(glm::mat4{1.f}, glm::vec3{0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 1.2f});
    renderer.drawObject(cubeModelMatrix, cubeMesh);
    glDisable(GL_CULL_FACE);

    glm::mat4 treeModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(treeModelMatrix, treeMesh);

    glm::mat4 firehydrantModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(firehydrantModelMatrix, firehydrantMesh);

    glm::mat4 trafficconeModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(trafficconeModelMatrix, trafficconeMesh);

    glm::mat4 trashbagsModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(trashbagsModelMatrix, trashbagsMesh);

    glm::mat4 stopsignModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(stopsignModelMatrix, stopsignMesh);

    glm::mat4 carModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(carModelMatrix, carMesh);

    glm::mat4 trafficlightModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(trafficlightModelMatrix, trafficlightMesh);

    glm::mat4 benchModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(benchModelMatrix, benchMesh);

    glm::mat4 mailboxModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(mailboxModelMatrix, mailboxMesh);

    for (Boid &boid : swarm) {
      boid.move(swarm);
      glm::mat4 boidModelMatrix =
          glm::translate(glm::mat4{1.f}, boid.getPosition()) *
          computeRotationMatrix(boid.getVelocity()) *
          glm::scale(glm::mat4{1.f}, glm::vec3{2.f});
      renderer.drawObject(boidModelMatrix, boidMesh);
    }
  };

  renderer.ctx.imgui = [&]() {
    ImGui::Begin("Boid Parameters");
    Boid::initializeUIElements();
    ImGui::End();
    ImGui::Begin("Light Parameters");
    Renderer::initializeUIElements();
    ImGui::End();
  };

  renderer.handleLookAround();
  renderer.handleZoom();

  renderer.start();

  renderer.close();

  return 0;
}