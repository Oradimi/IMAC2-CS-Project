#define DOCTEST_CONFIG_IMPLEMENT

#include "boid.hpp"
#include "doctest/doctest.h"
#include "math.hpp"
#include "math_tests.hpp"
#include "primitives/objLoader.hpp"
#include "primitives/transform.hpp"
#include "renderer.hpp"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

enum SimSwitchTabs { BOID, LIGHT };

std::vector<Boid> initializeBoids(RandomMath &rand, int number) {
  std::vector<Boid> swarm;
  swarm.reserve(number);

  for (int i = 0; i < number; ++i) {
    swarm.emplace_back(rand);
  }

  return swarm;
}

int main() {
  // Run the tests
  if (doctest::Context{}.run() != 0)
    return EXIT_FAILURE;

  MathTests math_test;
  SimSwitchTabs switchTabs = BOID;
  RandomMath rand;

  // bool math_debug_mode = true;
  bool math_debug_mode = false;
  if (math_debug_mode)
    math_test.initiate();

  std::vector<Boid> swarm{initializeBoids(rand, 20)};

  Renderer renderer;

  RenderedObject boidMesh{loadOBJ("spaceship.obj"), "spaceship.png",
                          "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject cubeMesh{loadOBJ("insideOutCube.obj"), "Gray.png",
                          "3D.vs.glsl", "directionalLight.fs.glsl"};

  renderer.ctx.update = [&]() {
    renderer.clearAll();

    Transform commonTransform{{0.f, -Boid::getBounds(), 0.f},
                              {0.f, 0.f, 0.f},
                              Boid::getBounds() * 0.1f};

    Transform carTransform{{0.f, -Boid::getBounds(), 0.f},
                           {0.f, 0.f, 0.f},
                           Boid::getBounds() * 0.05f};

    glEnable(GL_CULL_FACE);
    Transform cubeTransform{
        {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, Boid::getBounds()};
    renderer.drawObject(cubeTransform.getTransform(), cubeMesh);
    glDisable(GL_CULL_FACE);

    for (Boid &boid : swarm) {
      boid.move(swarm);
      // Transform boidTransform{
      //     boid.getPosition(),
      //     velocityToRotationVector(boid.getVelocity(), {0.f, 0.f,
      //     0.f}), 1.f};
      glm::mat4 boidModelMatrix =
          glm::translate(glm::mat4{1.f}, boid.getPosition()) *
          computeRotationMatrix(boid.getVelocity()) *
          glm::scale(glm::mat4{1.f}, glm::vec3{1.f});
      renderer.drawObject(boidModelMatrix, boidMesh);
    }
  };

  renderer.ctx.imgui = [&]() {
    ImGui::Begin("Parameters");
    if (ImGui::Button("Boids", ImVec2(100.0f, 0.0f)))
      switchTabs = BOID;
    ImGui::SameLine(0.0, 2.0f);
    if (ImGui::Button("Lights", ImVec2(100.0f, 0.0f)))
      switchTabs = LIGHT;

    switch (switchTabs) {
    case BOID:
      Boid::initializeUIElements();
      break;
    case LIGHT:
      Renderer::initializeUIElements();
      break;
    }
    ImGui::End();

    if (math_debug_mode)
      math_test.displayTestsGUI();
  };

  renderer.handleLookAround();
  renderer.handleZoom();

  renderer.start();

  renderer.close();

  return 0;
}

// RenderedObject treeMesh{loadOBJ("tree.obj"), "Tree.png", "3D.vs.glsl",
//                         "directionalLight.fs.glsl"};

// RenderedObject firehydrantMesh{loadOBJ("firehydrant.obj"),
// "FireHydrant.png",
//                                 "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject trafficconeMesh{loadOBJ("trafficcone.obj"),
// "TrafficCone.png",
//                                 "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject trashbagsMesh{loadOBJ("trashbags.obj"), "TrashBag.png",
//                               "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject stopsignMesh{loadOBJ("stopsign.obj"), "StopSign.png",
//                             "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject trafficlightMesh{loadOBJ("trafficlight.obj"),
//                                 "TrafficLight.png", "3D.vs.glsl",
//                                 "directionalLight.fs.glsl"};

// RenderedObject benchMesh{loadOBJ("bench.obj"), "Bench.png", "3D.vs.glsl",
//                           "directionalLight.fs.glsl"};

// RenderedObject mailboxMesh{loadOBJ("Mailbox.obj"), "MailBox.png",
//                             "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject streetlightMesh{loadOBJ("streetlight.obj"),
// "StreetLight.png",
//                                 "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject buildingMesh{loadOBJ("building.obj"), "Building.png",
//                             "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject outdoorseatingMesh{loadOBJ("outdoorseating.obj"),
//                                   "OutdoorSeating.png", "3D.vs.glsl",
//                                   "directionalLight.fs.glsl"};

// RenderedObject garbagecanMesh{loadOBJ("garbagecan.obj"), "GarbageCan.png",
//                               "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject gasstationMesh{loadOBJ("gasstation.obj"), "GasStation.png",
//                               "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject busstopMesh{loadOBJ("busstop.obj"), "BusStation.png",
//                             "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject intersectionMesh{loadOBJ("intersection.obj"),
//                                 "Intersection.png", "3D.vs.glsl",
//                                 "directionalLight.fs.glsl"};

// RenderedObject spiderrobotMesh{loadOBJ("SpiderRobot.obj"),
// "spiderrobot.png",
//                                 "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject coffeeMesh{loadOBJ("coffee.obj"), "Coffee.png",
//                           "3D.vs.glsl", "directionalLight.fs.glsl"};

// RenderedObject carMesh{loadOBJ("car.obj"), "Car1.png", "3D.vs.glsl",
//                         "directionalLight.fs.glsl"};

// RenderedObject car2Mesh{loadOBJ("Car2.obj"), "Car.png", "3D.vs.glsl",
//                         "directionalLight.fs.glsl"};

// RenderedObject car3Mesh{loadOBJ("Car3.obj"), "Car.png", "3D.vs.glsl",
//                         "directionalLight.fs.glsl"};

// RenderedObject car4Mesh{loadOBJ("Car4.obj"), "Car.png", "3D.vs.glsl",
//                         "directionalLight.fs.glsl"};

// RenderedObject car5Mesh{loadOBJ("Car5.obj"), "Car.png", "3D.vs.glsl",
//                         "directionalLight.fs.glsl"};

// RenderedObject car6Mesh{loadOBJ("Car6.obj"), "Car.png", "3D.vs.glsl",
//                         "directionalLight.fs.glsl"};

// RenderedObject car7Mesh{loadOBJ("Car7.obj"), "Car.png", "3D.vs.glsl",
//                         "directionalLight.fs.glsl"};

// RenderedObject car8Mesh{loadOBJ("Car8.obj"), "Car.png", "3D.vs.glsl",
//                         "directionalLight.fs.glsl"};

// RenderedObject car9Mesh{loadOBJ("Car9.obj"), "Car.png", "3D.vs.glsl",
//                         "directionalLight.fs.glsl"};

// RenderedObject car10Mesh{loadOBJ("Car10.obj"), "Car.png",
// "3D.vs.glsl",
//                           "directionalLight.fs.glsl"};

// RenderedObject car11Mesh{loadOBJ("Car11.obj"), "Car.png",
// "3D.vs.glsl",
//                           "directionalLight.fs.glsl"};

// renderer.drawObject(commonTransform.getTransform(), treeMesh);
// renderer.drawObject(commonTransform.getTransform(), firehydrantMesh);
// renderer.drawObject(commonTransform.getTransform(), trafficconeMesh);
// renderer.drawObject(commonTransform.getTransform(), trashbagsMesh);
// renderer.drawObject(commonTransform.getTransform(), stopsignMesh);
// renderer.drawObject(commonTransform.getTransform(), trafficlightMesh);
// renderer.drawObject(commonTransform.getTransform(), benchMesh);
// renderer.drawObject(commonTransform.getTransform(), mailboxMesh);
// renderer.drawObject(commonTransform.getTransform(), streetlightMesh);
// renderer.drawObject(commonTransform.getTransform(), buildingMesh);
// renderer.drawObject(commonTransform.getTransform(), outdoorseatingMesh);
// renderer.drawObject(commonTransform.getTransform(), garbagecanMesh);
// renderer.drawObject(commonTransform.getTransform(), gasstationMesh);
// renderer.drawObject(commonTransform.getTransform(), busstopMesh);
// renderer.drawObject(commonTransform.getTransform(), intersectionMesh);
// renderer.drawObject(commonTransform.getTransform(), spiderrobotMesh);
// renderer.drawObject(commonTransform.getTransform(), coffeeMesh);

// renderer.drawObject(carTransform.getTransform(), carMesh);
// renderer.drawObject(carTransform.getTransform(), car2Mesh);
// renderer.drawObject(carTransform.getTransform(), car3Mesh);
// renderer.drawObject(carTransform.getTransform(), car4Mesh);
// renderer.drawObject(carTransform.getTransform(), car5Mesh);
// renderer.drawObject(carTransform.getTransform(), car6Mesh);
// renderer.drawObject(carTransform.getTransform(), car7Mesh);
// renderer.drawObject(carTransform.getTransform(), car8Mesh);
// renderer.drawObject(carTransform.getTransform(), car9Mesh);
// renderer.drawObject(carTransform.getTransform(), car10Mesh);
// renderer.drawObject(carTransform.getTransform(), car11Mesh);
