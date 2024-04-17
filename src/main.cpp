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

  RenderedObject treeMesh{loadOBJ("tree.obj"), "Tree.png", "3D.vs.glsl",
                          "directionalLight.fs.glsl"};

  RenderedObject firehydrantMesh{loadOBJ("firehydrant.obj"), "FireHydrant.png",
                                 "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject trafficconeMesh{loadOBJ("trafficcone.obj"), "TrafficCone.png",
                                 "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject trashbagsMesh{loadOBJ("trashbags.obj"), "TrashBag.png",
                               "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject stopsignMesh{loadOBJ("stopsign.obj"), "StopSign.png",
                              "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject trafficlightMesh{loadOBJ("trafficlight.obj"),
                                  "TrafficLight.png", "3D.vs.glsl",
                                  "directionalLight.fs.glsl"};

  RenderedObject benchMesh{loadOBJ("bench.obj"), "Bench.png", "3D.vs.glsl",
                           "directionalLight.fs.glsl"};

  RenderedObject mailboxMesh{loadOBJ("Mailbox.obj"), "MailBox.png",
                             "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject streetlightMesh{loadOBJ("streetlight.obj"), "StreetLight.png",
                                 "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject buildingMesh{loadOBJ("building.obj"), "Building.png",
                              "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject outdoorseatingMesh{loadOBJ("outdoorseating.obj"),
                                    "OutdoorSeating.png", "3D.vs.glsl",
                                    "directionalLight.fs.glsl"};

  RenderedObject garbagecanMesh{loadOBJ("garbagecan.obj"), "GarbageCan.png",
                                "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject gasstationMesh{loadOBJ("gasstation.obj"), "GasStation.png",
                                "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject busstopMesh{loadOBJ("busstop.obj"), "BusStation.png",
                             "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject intersectionMesh{loadOBJ("intersection.obj"),
                                  "Intersection.png", "3D.vs.glsl",
                                  "directionalLight.fs.glsl"};

  RenderedObject spiderrobotMesh{loadOBJ("SpiderRobot.obj"), "spiderrobot.png",
                                 "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject carMesh{loadOBJ("car.obj"), "Car1.png", "3D.vs.glsl",
                         "directionalLight.fs.glsl"};

  RenderedObject car2Mesh{loadOBJ("Car2.obj"), "cartexture.png", "3D.vs.glsl",
                          "directionalLight.fs.glsl"};

  RenderedObject car3Mesh{loadOBJ("Car3.obj"), "cartexture.png", "3D.vs.glsl",
                          "directionalLight.fs.glsl"};

  RenderedObject car4Mesh{loadOBJ("Car4.obj"), "cartexture.png", "3D.vs.glsl",
                          "directionalLight.fs.glsl"};

  RenderedObject car5Mesh{loadOBJ("Car5.obj"), "cartexture.png", "3D.vs.glsl",
                          "directionalLight.fs.glsl"};

  RenderedObject car6Mesh{loadOBJ("Car6.obj"), "cartexture.png", "3D.vs.glsl",
                          "directionalLight.fs.glsl"};

  RenderedObject car7Mesh{loadOBJ("Car7.obj"), "cartexture.png", "3D.vs.glsl",
                          "directionalLight.fs.glsl"};

  RenderedObject car8Mesh{loadOBJ("Car8.obj"), "cartexture.png", "3D.vs.glsl",
                          "directionalLight.fs.glsl"};

  RenderedObject car9Mesh{loadOBJ("Car9.obj"), "cartexture.png", "3D.vs.glsl",
                          "directionalLight.fs.glsl"};

  RenderedObject car10Mesh{loadOBJ("Car10.obj"), "cartexture.png", "3D.vs.glsl",
                           "directionalLight.fs.glsl"};

  RenderedObject car11Mesh{loadOBJ("Car11.obj"), "cartexture.png", "3D.vs.glsl",
                           "directionalLight.fs.glsl"};

  RenderedObject coffeeMesh{loadOBJ("coffee.obj"), "cartexture.png",
                            "3D.vs.glsl", "directionalLight.fs.glsl"};

  RenderedObject alienMesh{loadOBJ("alien.obj"), "Alien.png", "3D.vs.glsl",
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

    glm::mat4 streetlightModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(streetlightModelMatrix, streetlightMesh);

    glm::mat4 buildingModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(buildingModelMatrix, buildingMesh);

    glm::mat4 outdoorseatingModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(outdoorseatingModelMatrix, outdoorseatingMesh);

    glm::mat4 garbagecanModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(garbagecanModelMatrix, garbagecanMesh);

    glm::mat4 gasstationModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(gasstationModelMatrix, gasstationMesh);

    glm::mat4 busstopModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{5.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(busstopModelMatrix, busstopMesh);

    glm::mat4 intersectionModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(intersectionModelMatrix, intersectionMesh);

    glm::mat4 spiderrobotModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(spiderrobotModelMatrix, spiderrobotMesh);

    glm::mat4 carModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 0.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.1f});
    renderer.drawObject(carModelMatrix, carMesh);

    glm::mat4 car2ModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(car2ModelMatrix, car2Mesh);

    glm::mat4 car3ModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(car3ModelMatrix, car3Mesh);

    glm::mat4 car4ModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(car4ModelMatrix, car4Mesh);

    glm::mat4 car5ModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(car5ModelMatrix, car5Mesh);

    glm::mat4 car6ModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(car6ModelMatrix, car6Mesh);

    glm::mat4 car7ModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(car7ModelMatrix, car7Mesh);

    glm::mat4 car8ModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(car8ModelMatrix, car8Mesh);

    glm::mat4 car9ModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(car9ModelMatrix, car9Mesh);

    glm::mat4 car10ModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(car10ModelMatrix, car10Mesh);

    glm::mat4 car11ModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(car11ModelMatrix, car11Mesh);

    glm::mat4 coffeeModelMatrix =
        glm::translate(glm::mat4{1.f},
                       glm::vec3{0.f, -Boid::getBounds() * 1.2f, 30.f}) *
        glm::scale(glm::mat4{1.f}, glm::vec3{Boid::getBounds() * 0.05f});
    renderer.drawObject(coffeeModelMatrix, coffeeMesh);

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