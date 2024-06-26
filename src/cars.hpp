#pragma once

#include "primitives/math.hpp"
#include "primitives/objLoader.hpp"
#include "primitives/object.hpp"
#include "primitives/transform.hpp"
#include "renderer.hpp"
#include <string>
#include <vector>

class Cars {
private:
  RandomMath rand;
  std::vector<RenderedObject> carList;
  float timer = 4.f;
  bool car_on_road = false;
  int random_car = -1;
  float car_speed = 30.f;
  float car_position = -100.f; // Initial car position
  int car_direction = 1;       // 1 for right, -1 for left
  bool horizontal_road = true; // Controls road orientation
  float car_height = 0.f;

  float positionX = horizontal_road ? car_position : -12.f;
  float positionZ = horizontal_road ? 12.f : car_position;

  void loadCarObjects() {
    carList.emplace_back(loadOBJ("car.obj"), "Car1.png", "3D.vs.glsl",
                         "light.fs.glsl");
    for (int i = 2; i < 12; ++i) {
      carList.emplace_back(loadOBJ("Car" + std::to_string(i) + ".obj"),
                           "Car.png", "3D.vs.glsl", "light.fs.glsl");
    }
  }

public:
  Cars() { loadCarObjects(); }

  void carEvents(const Renderer &renderer, float deltaTime, float waterHeight) {
    car_height = waterHeight > 6.f ? waterHeight - 6.f : 0.f;
    if (car_on_road) {
      if (car_height <= 0.f) {
        car_position += deltaTime * car_speed;
        positionX = horizontal_road ? car_position : -12.f;
        positionZ = horizontal_road ? 12.f : car_position;
      }

      Transform carTransform{
          {car_direction * positionX, car_height, car_direction * positionZ},
          {0.f, car_direction * 90.f + (horizontal_road ? 0.f : -90.f), 0.f},
          10.f};

      renderer.drawObject(carTransform.getTransform(), carList[random_car]);

      if (car_position > (Boid::getBounds() + 40.f)) {
        car_position = -Boid::getBounds() - 40.f;
        car_on_road = false;
      }
    } else {
      if (waterHeight < 0.f)
        timer -= deltaTime;
    }

    if (waterHeight > 0.f)
      return;

    if (timer <= 0.f) {
      car_on_road = true;
      random_car = rand.generateUniformDiscrete(0, carList.size());
      car_speed = rand.generateLaplace(120.0, 5.0);
      timer = rand.generateExponential(0.6);
      car_direction = rand.generateBernoulli(0.5) ? 1 : -1;
      horizontal_road = rand.generateBernoulli(0.5);
    }
  }

  float getCarHeight() const { return car_height; }

  RenderedObject *getCar(int index) { return &carList[index]; }
};
