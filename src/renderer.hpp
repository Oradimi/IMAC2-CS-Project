#pragma once

#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glimac/cone_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include "primitives/texture.hpp"
#include "primitives/object.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

std::vector<glm::vec3> createRandomlySpreadSpheres();

struct EarthProgram {
  Object earthObject;
  p6::Shader _Program;

  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;
  GLint uEarthTexture;
  GLint uCloudTexture;

  EarthProgram()
      :earthObject(glimac::sphere_vertices(-1.f, 32, 16)),
      _Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl")} {
    uMVPMatrix = glGetUniformLocation(_Program.id(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(_Program.id(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(_Program.id(), "uNormalMatrix");
    uEarthTexture = glGetUniformLocation(_Program.id(), "uEarthTexture");
    uCloudTexture = glGetUniformLocation(_Program.id(), "uCloudTexture");

    earthObject.addTexture("EarthMap.jpg");
    earthObject.addTexture("CloudMap.jpg");

    earthObject.defineVBO();
    earthObject.defineVAO();

    earthObject.defineTextures();
  }
};

struct MoonProgram {
  Object moonObject;
  p6::Shader _Program;

  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;
  GLint uTexture;

  MoonProgram()
      :moonObject(glimac::cone_vertices(2.f, 2.f, 10.f, 10.f)),
      _Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl")} {
    uMVPMatrix = glGetUniformLocation(_Program.id(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(_Program.id(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(_Program.id(), "uNormalMatrix");
    uTexture = glGetUniformLocation(_Program.id(), "uTexture");

    moonObject.addTexture("MoonMap.jpg");

    moonObject.defineVBO();
    moonObject.defineVAO();

    moonObject.defineTextures();
  }
};

class Renderer {
private:
  std::vector<Object> objectList;

public:
  p6::Context ctx = p6::Context{{.title = "Amazing Oradimi Parzi_Val Boids"}};
  
  TrackballCamera camera;

  Renderer();

  void addObject(Object& object);

  void clearAll();

  void handleLookAround();

  void handleZoom();

  void drawEarth(EarthProgram& earthProgram) const;

  void drawMoon(MoonProgram& moonProgram, std::vector<glm::vec3> randomAxes);

  void start() { ctx.start(); };
};
