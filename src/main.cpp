#define DOCTEST_CONFIG_IMPLEMENT

#include "boid.hpp"
#include "cars.hpp"
#include "doctest/doctest.h"
#include "primitives/math.hpp"
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

std::vector<Boid> initializeBoids(RandomMath &rand, const int &number) {
  std::vector<Boid> swarm;
  int boidCount = static_cast<int>(rand.generateBeta(2.0, 2.0) * 60.f) + 20;
  swarm.reserve(boidCount);

  for (int i = 0; i < boidCount; ++i) {
    swarm.emplace_back(rand);
  }

  return swarm;
}

void handleWaveEvent(Transform &wave, MarkovState &newState,
                     MarkovState &currentState) {
  if (newState != currentState) {
    switch (newState) {
    case LOW:
      wave.move({0.f, -20.f, 0.f});
      break;
    case HIGH:
      switch (currentState) {
      case LOW:
        wave.move({0.f, +20.f, 0.f});
        break;
      case HIGH:
        break;
      case FLOOD:
        wave.move({0.f, -40.f, 0.f});
        break;
      }
      break;
    case FLOOD:
      wave.move({0.f, +40.f, 0.f});
      break;
    }
    currentState = newState;
  }
}

int main() {
  // Run the tests
  if (doctest::Context{}.run() != 0)
    return EXIT_FAILURE;

  RandomMath rand;

  std::vector<Boid> swarm{initializeBoids(rand, 20)};

  Renderer renderer;

  RenderedObject boidMesh{loadOBJ("spaceship.obj"), "spaceship.png",
                          "3D.vs.glsl", "light.fs.glsl"};

  RenderedObject cubeMesh{loadOBJ("insideOutCube.obj"), "textureCube.png",
                          "3D.vs.glsl", "light.fs.glsl"};
  cubeMesh.Ks = 1.f;
  cubeMesh.Shininess = 1.f;

  RenderedObject intersectionMesh{loadOBJ("intersection.obj"),
                                  "Intersection.png", "3D.vs.glsl",
                                  "light.fs.glsl"};

  Transform intersectionTransform{{1.f, 1.f, 0.f}, {0.f, 0.f, 0.f}, 41.f};

  RenderedObject streetlightMesh{loadOBJ("streetlight.obj"), "StreetLight.png",
                                 "3D.vs.glsl", "light.fs.glsl"};

  Transform streetLightTransform_1{{-60.f, 0.f, -30.f}, {0.f, 0.f, 0.f}, 2.f};
  Transform streetLightTransform_2{{60.f, 0.f, 30.f}, {0.f, 180.f, 0.f}, 2.f};

  renderer.addLight(
      {streetLightTransform_1.getPosition() + glm::vec3{0.f, 18.f, 6.f},
       {400.f, 400.f, 100.f}});
  renderer.addLight(
      {streetLightTransform_2.getPosition() + glm::vec3{0.f, 18.f, -6.f},
       {400.f, 400.f, 100.f}});

  RenderedObject spiderrobotMesh{loadOBJ("SpiderRobot.obj"), "spiderrobot.png",
                                 "3D.vs.glsl", "light.fs.glsl"};

  RenderedObject treeMesh{loadOBJ("tree.obj"), "Tree.png", "3D.vs.glsl",
                          "light.fs.glsl"};

  RenderedObject firehydrantMesh{loadOBJ("firehydrant.obj"), "FireHydrant.png",
                                 "3D.vs.glsl", "light.fs.glsl"};

  RenderedObject buildingMesh{loadOBJ("building.obj"), "Building.png",
                              "3D.vs.glsl", "light.fs.glsl"};

  RenderedObject coffeeMesh{loadOBJ("coffee.obj"), "Coffee.png", "3D.vs.glsl",
                            "light.fs.glsl"};

  Transform treeTransform{{50.f, 0.f, 80.f}, {0.f, 60.f, 0.f}, 4.f};

  Transform firehydrantTransform{{40.f, 2.f, 30.f}, {0.f, 180.f, 0.f}, 1.f};

  Transform buildingTransform{{90.f, 1.f, 80.f}, {0.f, -112.5f, 0.f}, 6.f};

  Transform buildingTransform_2{{-90.f, 1.f, -80.f}, {0.f, 45.f, 0.f}, 6.f};

  Transform coffeeTransform{{-85.f, 1.f, 80.f}, {0.f, 130.f, 0.f}, 6.f};

  Cars cars;

  Transform wavesTransform{{0.f, -20.f, 0.f}, {0.f, 0.f, 0.f}, 2.f};

  RenderedObject wavesMesh{loadOBJ("waves.obj"), "Water.png", "3D.vs.glsl",
                           "light.fs.glsl"};

  std::unordered_map<std::pair<float, float>, float, HashPair> waveOffsets;

  glm::mat3 markovMatrix = {
      {0.98f, 0.02f, 0.f}, {0.03f, 0.96f, 0.01f}, {0.f, 0.02f, 0.98f}};
  MarkovState currentState = HIGH;
  MarkovState newState = HIGH;

  renderer.ctx.update = [&]() {
    renderer.clearAll();

    // DECORATIVE
    renderer.drawObject(streetLightTransform_1.getTransform(), streetlightMesh);
    renderer.drawObject(streetLightTransform_2.getTransform(), streetlightMesh);

    renderer.drawObject(intersectionTransform.getTransform(), intersectionMesh);
    renderer.drawObject(treeTransform.getTransform(), treeMesh);
    renderer.drawObject(firehydrantTransform.getTransform(), firehydrantMesh);
    renderer.drawObject(buildingTransform.getTransform(), buildingMesh);
    renderer.drawObject(buildingTransform_2.getTransform(), buildingMesh);
    renderer.drawObject(coffeeTransform.getTransform(), coffeeMesh);

    // RANDOM ELEMENTS
    cars.carEvents(renderer, renderer.ctx.delta_time(),
                   wavesTransform.getPosition().y);

    wavesMesh.updateWave(rand, renderer.ctx, waveOffsets);
    if (rand.generateBinomial(0.4, 3) == 3) {
      newState = rand.getNextMarkovState(markovMatrix, currentState);
      handleWaveEvent(wavesTransform, newState, currentState);
    }
    renderer.drawObject(wavesTransform.getTransform(), wavesMesh);
    wavesTransform.easeToTargetPosition(renderer.ctx.delta_time(), 0.4f);

    // ESSENTIALS
    Transform wandererTransform{renderer.camera.getWandererTransform()};
    if (renderer.camera.cameraMode == FREEFLY) {
      renderer.drawObject(wandererTransform.getTransform(), spiderrobotMesh);
    }

    glEnable(GL_CULL_FACE);
    Transform cubeTransform{
        {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, Boid::getBounds()};
    renderer.drawObject(cubeTransform.getTransform(), cubeMesh);
    glDisable(GL_CULL_FACE);

    for (Boid &boid : swarm) {
      boid.move(swarm);
      glm::mat4 boidModelMatrix =
          glm::translate(glm::mat4{1.f}, boid.getPosition()) *
          computeRotationMatrix(boid.getVelocity()) *
          glm::scale(glm::mat4{1.f}, glm::vec3{1.f});
      renderer.drawObject(boidModelMatrix, boidMesh);
    }

    renderer.handleInputs();
  };

  renderer.renderUI();

  renderer.start();

  renderer.close();

  return 0;
}

// RenderedObject trafficconeMesh{loadOBJ("trafficcone.obj"),
// "TrafficCone.png",
//                                 "3D.vs.glsl", "light.fs.glsl"};

// RenderedObject trashbagsMesh{loadOBJ("trashbags.obj"), "TrashBag.png",
//                               "3D.vs.glsl", "light.fs.glsl"};

// RenderedObject stopsignMesh{loadOBJ("stopsign.obj"), "StopSign.png",
//                             "3D.vs.glsl", "light.fs.glsl"};

// RenderedObject trafficlightMesh{loadOBJ("trafficlight.obj"),
//                                 "TrafficLight.png", "3D.vs.glsl",
//                                 "light.fs.glsl"};

// RenderedObject benchMesh{loadOBJ("bench.obj"), "Bench.png", "3D.vs.glsl",
//                           "light.fs.glsl"};

// RenderedObject mailboxMesh{loadOBJ("Mailbox.obj"), "MailBox.png",
//                             "3D.vs.glsl", "light.fs.glsl"};

// RenderedObject streetlightMesh{loadOBJ("streetlight.obj"),
// "StreetLight.png",
//                                 "3D.vs.glsl", "light.fs.glsl"};

// RenderedObject outdoorseatingMesh{loadOBJ("outdoorseating.obj"),
//                                   "OutdoorSeating.png", "3D.vs.glsl",
//                                   "light.fs.glsl"};

// RenderedObject garbagecanMesh{loadOBJ("garbagecan.obj"), "GarbageCan.png",
//                               "3D.vs.glsl", "light.fs.glsl"};

// RenderedObject gasstationMesh{loadOBJ("gasstation.obj"), "GasStation.png",
//                               "3D.vs.glsl", "light.fs.glsl"};

// RenderedObject busstopMesh{loadOBJ("busstop.obj"), "BusStation.png",
//                             "3D.vs.glsl", "light.fs.glsl"};

// RenderedObject intersectionMesh{loadOBJ("intersection.obj"),
//                                 "Intersection.png", "3D.vs.glsl",
//                                 "light.fs.glsl"};

// RenderedObject coffeeMesh{loadOBJ("coffee.obj"), "Coffee.png",
//                           "3D.vs.glsl", "light.fs.glsl"};

// RenderedObject carMesh{loadOBJ("car.obj"), "Car1.png", "3D.vs.glsl",
//                         "light.fs.glsl"};

// RenderedObject car2Mesh{loadOBJ("Car2.obj"), "Car.png", "3D.vs.glsl",
//                         "light.fs.glsl"};

// RenderedObject car3Mesh{loadOBJ("Car3.obj"), "Car.png", "3D.vs.glsl",
//                         "light.fs.glsl"};

// RenderedObject car4Mesh{loadOBJ("Car4.obj"), "Car.png", "3D.vs.glsl",
//                         "light.fs.glsl"};

// RenderedObject car5Mesh{loadOBJ("Car5.obj"), "Car.png", "3D.vs.glsl",
//                         "light.fs.glsl"};

// RenderedObject car6Mesh{loadOBJ("Car6.obj"), "Car.png", "3D.vs.glsl",
//                         "light.fs.glsl"};

// RenderedObject car7Mesh{loadOBJ("Car7.obj"), "Car.png", "3D.vs.glsl",
//                         "light.fs.glsl"};

// RenderedObject car8Mesh{loadOBJ("Car8.obj"), "Car.png", "3D.vs.glsl",
//                         "light.fs.glsl"};

// RenderedObject car9Mesh{loadOBJ("Car9.obj"), "Car.png", "3D.vs.glsl",
//                         "light.fs.glsl"};

// RenderedObject car10Mesh{loadOBJ("Car10.obj"), "Car.png",
// "3D.vs.glsl",
//                           "light.fs.glsl"};

// RenderedObject car11Mesh{loadOBJ("Car11.obj"), "Car.png",
// "3D.vs.glsl",
//                           "light.fs.glsl"};

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
