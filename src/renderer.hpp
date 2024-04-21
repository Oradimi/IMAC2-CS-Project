#pragma once

#include "camera/TrackballCamera.hpp"
#include "implot/implot.h"
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

class Renderer {
private:
  std::vector<RenderedObject> objectList;
  std::vector<Light> lightList;

  static float uWorldLightIntensity;
  static glm::vec3 lightDir;

public:
  p6::Context ctx = p6::Context{{.width = 1600,
                                 .height = 900,
                                 .title = "Amazing Oradimi Parzi_Val Boids"}};

  TrackballCamera camera;

  Renderer();

  void addObject(RenderedObject object);

  void addLight(Light light);

  void clearAll();

  void handleLookAround();

  void handleZoom();

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
