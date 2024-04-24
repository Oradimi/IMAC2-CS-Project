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

std::vector<Boid> initializeBoids(RandomMath &rand, const int &minCount) {
  std::vector<Boid> swarm;
  int boidCount =
      static_cast<int>(rand.generateBeta(2.0, 2.0) * 60.f) + minCount;
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
      wave.moveEase({0.f, -20.f, 0.f});
      break;
    case HIGH:
      switch (currentState) {
      case LOW:
        wave.moveEase({0.f, +20.f, 0.f});
        break;
      case HIGH:
        break;
      case FLOOD:
        wave.moveEase({0.f, -40.f, 0.f});
        break;
      }
      break;
    case FLOOD:
      wave.moveEase({0.f, +40.f, 0.f});
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

  // Boids
  RenderedObject boidMesh_1{loadOBJ("spaceship.obj"), "spaceship.png",
                            "3D.vs.glsl", "light.fs.glsl"};

  RenderedObject boidMesh_2{loadOBJ("spaceship2.obj"), "spaceship.png",
                            "3D.vs.glsl", "light.fs.glsl"};

  RenderedObject boidMesh_3{loadOBJ("spaceship3.obj"), "spaceship.png",
                            "3D.vs.glsl", "light.fs.glsl"};

  // Wanderer
  RenderedObject spiderrobotMesh{loadOBJ("SpiderRobot.obj"), "spiderrobot.png",
                                 "3D.vs.glsl", "light.fs.glsl"};

  // Bounds
  RenderedObject cubeMesh{loadOBJ("insideOutCube.obj"), "textureCube.png",
                          "3D.vs.glsl", "light.fs.glsl"};

  // Decorations
  RenderedObject intersectionMesh{loadOBJ("intersection.obj"),
                                  "Intersection.png", "3D.vs.glsl",
                                  "light.fs.glsl"};
  Transform intersectionTransform{{1.f, 1.f, 0.f}, {0.f, 0.f, 0.f}, 41.f};

  RenderedObject streetlightMesh{loadOBJ("streetlight.obj"), "StreetLight.png",
                                 "3D.vs.glsl", "light.fs.glsl"};

  Transform streetLightTransform_1{{-60.f, 0.f, -32.5f}, {0.f, 0.f, 0.f}, 2.5f};
  Transform streetLightTransform_2{{60.f, 0.f, 32.5f}, {0.f, 180.f, 0.f}, 2.5f};

  renderer.addLight(
      {streetLightTransform_1.getPosition() + glm::vec3{0.f, 18.f, 6.f},
       {400.f, 400.f, 100.f}});
  renderer.addLight(
      {streetLightTransform_2.getPosition() + glm::vec3{0.f, 18.f, -6.f},
       {400.f, 400.f, 100.f}});

  RenderedObject treeMesh{loadOBJ("tree.obj"), "Tree.png", "3D.vs.glsl",
                          "light.fs.glsl"};
  Transform treeTransform_1{{100.f, 0.f, 50.f}, {0.f, 240.f, 0.f}, 4.f};
  Transform treeTransform_2{{-100.f, 0.f, -50.f}, {0.f, 205.f, 0.f}, 5.f};

  RenderedObject firehydrantMesh{loadOBJ("firehydrant.obj"), "FireHydrant.png",
                                 "3D.vs.glsl", "light.fs.glsl"};
  Transform firehydrantTransform{{40.f, 2.f, 30.f}, {0.f, 180.f, 0.f}, 1.f};

  RenderedObject buildingMesh_1{loadOBJ("building.obj"), "Building.png",
                                "3D.vs.glsl", "light.fs.glsl"};
  Transform buildingTransform_1{{90.f, 1.f, 88.f}, {0.f, -134.5f, 0.f}, 6.f};

  RenderedObject buildingMesh_2{loadOBJ("building2.obj"), "Building2.png",
                                "3D.vs.glsl", "light.fs.glsl"};
  Transform buildingTransform_2{{-90.f, 1.f, -88.f}, {0.f, 43.75f, 0.f}, 6.f};

  RenderedObject coffeeMesh{loadOBJ("coffee.obj"), "car.png", "3D.vs.glsl",
                            "light.fs.glsl"};

  Transform coffeeTransform{{-95.f, -4.f, 80.f}, {0.f, 90.f, 0.f}, 6.f};

  RenderedObject outdoorseatingMesh{loadOBJ("outdoorseating.obj"),
                                    "OutdoorSeating.png", "3D.vs.glsl",
                                    "light.fs.glsl"};
  Transform outdoorseatingTransform{{-55.f, 0.f, 95.f}, {0.f, 140.f, 0.f}, 3.f};

  RenderedObject benchMesh{loadOBJ("bench.obj"), "Bench.png", "3D.vs.glsl",
                           "light.fs.glsl"};

  Transform benchTransform{{-52.f, 0.f, 54.f}, {0.f, 135.f, 0.f}, 5.f};

  RenderedObject garbagecanMesh{loadOBJ("garbagecan.obj"), "GarbageCan.png",
                                "3D.vs.glsl", "light.fs.glsl"};

  Transform garbagecanTransform_1{{-95.f, 0.f, 50.f}, {0.f, 97.f, 0.f}, 3.f};
  Transform garbagecanTransform_2{{60.f, 0.f, 113.f}, {0.f, -5.f, 0.f}, 3.f};

  RenderedObject trashbagsMesh{loadOBJ("trashbags.obj"), "TrashBag.png",
                               "3D.vs.glsl", "light.fs.glsl"};

  Transform trashbagsTransform_1{{-90.f, 0.f, 45.f}, {0.f, -5.f, 0.f}, 3.f};
  Transform trashbagsTransform_2{{-60.f, 0.f, -113.f}, {0.f, -5.f, 0.f}, 3.f};

  RenderedObject trafficlightMesh{loadOBJ("trafficlight.obj"),
                                  "TrafficLight.png", "3D.vs.glsl",
                                  "light.fs.glsl"};

  Transform trafficlightTransform_1{{-33.f, 2.f, 32.5f}, {0.f, 45.f, 0.f}, 3.f};
  Transform trafficlightTransform_2{
      {33.f, 2.f, -32.5f}, {0.f, -135.f, 0.f}, 3.f};

  RenderedObject stopsignMesh{loadOBJ("stopsign.obj"), "StopSign.png",
                              "3D.vs.glsl", "light.fs.glsl"};

  Transform stopsignTransform_1{{-33.f, 2.f, -32.5f}, {0.f, 135.f, 0.f}, 3.f};
  Transform stopsignTransform_2{{33.f, 2.f, 32.5f}, {0.f, -45.f, 0.f}, 3.f};

  RenderedObject mailboxMesh_1{loadOBJ("Mailbox.obj"), "MailBox.png",
                               "3D.vs.glsl", "light.fs.glsl"};

  Transform mailboxTransform_1{{-85.f, 0.f, -55.f}, {0.f, 223.75f, 0.f}, 4.f};

  RenderedObject mailboxMesh_2{loadOBJ("Mailbox.obj"), "MailBox2.png",
                               "3D.vs.glsl", "light.fs.glsl"};

  Transform mailboxTransform_2{{85.f, 0.f, 55.f}, {0.f, 45.5f, 0.f}, 4.f};

  RenderedObject gasstationMesh{loadOBJ("gasstation.obj"), "GasStation.png",
                                "3D.vs.glsl", "light.fs.glsl"};

  Transform gasstationTransform{{80.f, 0.f, -115.f}, {0.f, 0.f, 0.f}, 5.f};

  RenderedObject busstopMesh{loadOBJ("busstop.obj"), "BusStation.png",
                             "3D.vs.glsl", "light.fs.glsl"};

  Transform busstopTransform{{80.f, 0.f, -55.f}, {0.f, 0.f, 0.f}, 2.5f};

  RenderedObject trafficconeMesh{loadOBJ("trafficcone.obj"), "TrafficCone.png",
                                 "3D.vs.glsl", "light.fs.glsl"};

  Transform trafficconeTransform_1{{110.f, 0.f, -55.f}, {0.f, 180.f, 0.f}, 4.f};
  Transform trafficconeTransform_2{{50.f, 0.f, -55.f}, {0.f, 180.f, 0.f}, 4.f};

  RenderedObject flyingsaucerMesh{loadOBJ("flyingsaucer.obj"),
                                  "SaucerBaseColor.png", "3D.vs.glsl",
                                  "light.fs.glsl"};

  Transform flyingsaucerTransform{{90.f, 75.f, 88.f}, {0.f, -134.5f, 0.f}, 2.f};

  RenderedObject alienMesh{loadOBJ("alien.obj"), "Alien.png", "3D.vs.glsl",
                           "light.fs.glsl"};

  Transform alienTransform{{-55.f, 9.f, 75.f}, {0.f, 100.f, 0.f}, 3.f};

  RenderedObject meinaMesh{loadOBJ("meina.obj"), "MeinaDiffuse.png",
                           "3D.vs.glsl", "light.fs.glsl"};

  Transform meinaTransform{{90.f, 1.f, 88.f}, {0.f, -134.5f, 0.f}, 13.f};

  Transform car8Transform{{100.f, 0.f, -85.f}, {0.f, 180.f, 0.f}, 9.f};

  Transform car9Transform{{60.f, 0.f, -85.f}, {0.f, 180.f, 0.f}, 9.f};

  Transform car10Transform{{50.f, 0.f, 80.f}, {0.f, 180.f, 0.f}, 9.f};

  Transform car11Transform{{-50.f, 0.f, -80.f}, {0.f, 180.f, 0.f}, 9.f};

  Cars cars;

  RenderedObject wavesMesh{loadOBJ("waves.obj"), "Water.png", "3D.vs.glsl",
                           "light.fs.glsl"};
  Transform wavesTransform{{0.f, -20.f, 0.f}, {0.f, 0.f, 0.f}, 2.f};
  std::unordered_map<std::pair<float, float>, float, HashPair> waveOffsets;
  glm::mat3 waveStateTransitions = {
      {0.98f, 0.02f, 0.f}, {0.03f, 0.96f, 0.01f}, {0.f, 0.02f, 0.98f}};
  MarkovState currentWaveState = HIGH;
  MarkovState newWaveState = HIGH;

  renderer.ctx.update = [&]() {
    renderer.clearAll();

    glm::vec3 car_height{0.f, cars.getCarHeight(), 0.f};

    // DECORATIVE
    renderer.drawObject(streetLightTransform_1.getTransform(), streetlightMesh);
    renderer.drawObject(streetLightTransform_2.getTransform(), streetlightMesh);

    renderer.drawObject(intersectionTransform.getTransform(), intersectionMesh);

    renderer.drawObject(treeTransform_1.getTransform(), treeMesh);
    renderer.drawObject(treeTransform_2.getTransform(), treeMesh);

    renderer.drawObject(firehydrantTransform.getTransform(), firehydrantMesh);

    renderer.drawObject(buildingTransform_1.getTransform(), buildingMesh_1);
    renderer.drawObject(buildingTransform_2.getTransform(), buildingMesh_2);

    renderer.drawObject(coffeeTransform.getTransform(), coffeeMesh);

    renderer.drawObject(outdoorseatingTransform.getTransform(),
                        outdoorseatingMesh);

    renderer.drawObject(benchTransform.getTransform(), benchMesh);

    renderer.drawObject(garbagecanTransform_1.temporarySnap(car_height),
                        garbagecanMesh);
    renderer.drawObject(garbagecanTransform_2.temporarySnap(car_height),
                        garbagecanMesh);

    renderer.drawObject(trashbagsTransform_1.getTransform(), trashbagsMesh);
    renderer.drawObject(trashbagsTransform_2.getTransform(), trashbagsMesh);

    renderer.drawObject(trafficlightTransform_1.getTransform(),
                        trafficlightMesh);
    renderer.drawObject(trafficlightTransform_2.getTransform(),
                        trafficlightMesh);

    renderer.drawObject(stopsignTransform_1.getTransform(), stopsignMesh);
    renderer.drawObject(stopsignTransform_2.getTransform(), stopsignMesh);

    renderer.drawObject(mailboxTransform_1.getTransform(), mailboxMesh_1);
    renderer.drawObject(mailboxTransform_2.getTransform(), mailboxMesh_2);

    renderer.drawObject(gasstationTransform.getTransform(), gasstationMesh);

    renderer.drawObject(busstopTransform.getTransform(), busstopMesh);

    renderer.drawObject(trafficconeTransform_1.getTransform(), trafficconeMesh);
    renderer.drawObject(trafficconeTransform_2.getTransform(), trafficconeMesh);

    renderer.drawObject(flyingsaucerTransform.getTransform(), flyingsaucerMesh);

    renderer.drawObject(alienTransform.getTransform(), alienMesh);

    renderer.drawObject(meinaTransform.getTransform(), meinaMesh);

    renderer.drawObject(car8Transform.temporarySnap(car_height),
                        *cars.getCar(7));
    renderer.drawObject(car9Transform.temporarySnap(car_height),
                        *cars.getCar(8));
    renderer.drawObject(car10Transform.temporarySnap(car_height),
                        *cars.getCar(9));
    renderer.drawObject(car11Transform.temporarySnap(car_height),
                        *cars.getCar(10));

    // RANDOM ELEMENTS
    cars.carEvents(renderer, renderer.ctx.delta_time(),
                   wavesTransform.getPosition().y);

    wavesMesh.updateWave(rand, renderer.ctx, waveOffsets);
    if (rand.generateBinomial(0.4, 3) == 3) {
      newWaveState =
          rand.getNextMarkovState(waveStateTransitions, currentWaveState);
      handleWaveEvent(wavesTransform, newWaveState, currentWaveState);
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
      switch (static_cast<int>(Boid::getLevelOfDetail())) {
      case 3:
        renderer.drawObject(boidModelMatrix, boidMesh_1);
        break;
      case 2:
        renderer.drawObject(boidModelMatrix, boidMesh_2);
        break;
      case 1:
      default:
        renderer.drawObject(boidModelMatrix, boidMesh_3);
        break;
      }
    }

    renderer.handleInputs();
  };

  renderer.renderUI();

  renderer.start();

  renderer.close();

  return 0;
}