#pragma once

#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include "primitives/object.hpp"
#include "primitives/texture.hpp"
#include "programs.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Renderer {
private:
  std::vector<RenderedObject> objectList;

public:
  p6::Context ctx = p6::Context{{.title = "Amazing Oradimi Parzi_Val Boids"}};

  TrackballCamera camera;

  Renderer();

  void addObject(RenderedObject object);

  void clearAll();

  void handleLookAround();

  void handleZoom();

  void drawObject(glm::mat4 &modelMatrix, RenderedObject &object) const;

  void start() { ctx.start(); };

  void close() {
    for (RenderedObject &object : objectList) {
      object.clear();
    }
  };
};

glm::mat4 computeRotationMatrix(const glm::vec3 &velocity);
