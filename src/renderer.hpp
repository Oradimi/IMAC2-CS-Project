#pragma once

#include "camera.hpp"
#include "implot/implot.h"
#include "math_tests.hpp"
#include "p6/p6.h"
#include "primitives/object.hpp"
#include <basetsd.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Light {
  glm::vec3 position;
  glm::vec3 color;
};

enum SimSwitchTabs { BOID, LIGHT, WANDERER };

class Renderer {
private:
  std::vector<RenderedObject> objectList;
  std::vector<Light> lightList;

  MathTests math_test;
  bool math_debug_mode = false;
  SimSwitchTabs switchTabs = BOID;

  static float uWorldLightIntensity;
  static glm::vec3 lightDir;

public:
  p6::Context ctx = p6::Context{{.width = 1600,
                                 .height = 900,
                                 .title = "Amazing Oradimi Parzi_Val Boids"}};

  Camera camera;

  Renderer();

  void addObject(RenderedObject object);

  void addLight(Light light);

  void renderUI();

  void clearAll();

  void handleLookAround();

  void handleZoom();

  void handleKeyboard();

  void handleInputs();

  void drawObject(const glm::mat4 &modelMatrix,
                  const RenderedObject &object) const;

  static void initializeUIElements();

  void start() { ctx.start(); };

  void close() {
    for (RenderedObject &object : objectList) {
      ImPlot::DestroyContext();
      object.clear();
    }
  };
};
