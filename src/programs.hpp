#pragma once

#include "p6/p6.h"
#include "glimac/sphere_vertices.hpp"
#include "glimac/cone_vertices.hpp"
#include "primitives/texture.hpp"
#include "primitives/object.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

inline std::vector<glm::vec3> createRandomAxes() {
  std::vector<glm::vec3> randomAxes{glm::sphericalRand(2.f)};
  for (int i = 1; i < 32; i++) {
    randomAxes.emplace_back(glm::sphericalRand(2.f));
  }
  return randomAxes;
};

struct EarthProgram {
  Object _Object;
  p6::Shader _Program;

  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;
  GLint uEarthTexture;
  GLint uCloudTexture;

  EarthProgram()
      :_Object(glimac::sphere_vertices(-1.f, 32, 16)),
      _Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl")},
      uMVPMatrix(glGetUniformLocation(_Program.id(), "uMVPMatrix")),
      uMVMatrix(glGetUniformLocation(_Program.id(), "uMVMatrix")),
      uNormalMatrix(glGetUniformLocation(_Program.id(), "uNormalMatrix")),
      uEarthTexture(glGetUniformLocation(_Program.id(), "uEarthTexture")),
      uCloudTexture(glGetUniformLocation(_Program.id(), "uCloudTexture")) {

    _Object.addTexture("EarthMap.jpg");
    _Object.addTexture("CloudMap.jpg");

    _Object.defineVBO();
    _Object.defineVAO();

    _Object.defineTextures();
  }
};

struct MoonProgram {
  Object _Object;
  p6::Shader _Program;

  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;
  GLint uTexture;

  std::vector<glm::vec3> _randomAxes;

  MoonProgram()
      :_Object(glimac::cone_vertices(2.f, 2.f, 10.f, 10.f)),
      _Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl")},
      uMVPMatrix(glGetUniformLocation(_Program.id(), "uMVPMatrix")),
      uMVMatrix(glGetUniformLocation(_Program.id(), "uMVMatrix")),
      uNormalMatrix(glGetUniformLocation(_Program.id(), "uNormalMatrix")),
      uTexture(glGetUniformLocation(_Program.id(), "uTexture")) {
    
    _Object.addTexture("MoonMap.jpg");

    _Object.defineVBO();
    _Object.defineVAO();

    _Object.defineTextures();

    _randomAxes = createRandomAxes();
  }
};