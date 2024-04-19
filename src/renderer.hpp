#pragma once

#include "camera/TrackballCamera.hpp"
#include "p6/p6.h"
#include "primitives/object.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Renderer {
private:
  std::vector<RenderedObject> objectList;

  static float uKd;
  static float uKs;
  static float uLightIntensity;
  static float uShininess;
  static glm::vec3 lightDir;

public:
  p6::Context ctx = p6::Context{{.width = 1600,
                                 .height = 900,
                                 .title = "Amazing Oradimi Parzi_Val Boids"}};

  TrackballCamera camera;

  Renderer();

  void addObject(RenderedObject object);

  void clearAll();

  void handleLookAround();

  void handleZoom();

  void drawObject(const glm::mat4 &modelMatrix,
                  const RenderedObject &object) const;

  static void initializeUIElements();

  void start() { ctx.start(); };

  void close() {
    for (RenderedObject &object : objectList) {
      object.clear();
    }
  };
};
