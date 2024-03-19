#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "p6/p6.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

std::vector<glm::vec3> createRandomlySpreadSpheres();

struct EarthProgram {
  p6::Shader _Program;

  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;
  GLint uEarthTexture;
  GLint uCloudTexture;

  EarthProgram()
      : _Program{p6::load_shader("shaders/3D.vs.glsl",
                                 "shaders/multiTex3D.fs.glsl")} {
    uMVPMatrix = glGetUniformLocation(_Program.id(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(_Program.id(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(_Program.id(), "uNormalMatrix");
    uEarthTexture = glGetUniformLocation(_Program.id(), "uEarthTexture");
    uCloudTexture = glGetUniformLocation(_Program.id(), "uCloudTexture");
  }
};

struct MoonProgram {
  p6::Shader _Program;

  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;
  GLint uTexture;

  MoonProgram()
      : _Program{
            p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl")} {
    uMVPMatrix = glGetUniformLocation(_Program.id(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(_Program.id(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(_Program.id(), "uNormalMatrix");
    uTexture = glGetUniformLocation(_Program.id(), "uTexture");
  }
};

class Texture {
private:
  std::string path;

public:
  Texture(std::vector<GLuint> &textureIDList, std::string path);
};

class Renderer {
private:
  std::vector<Texture> textureList;
  GLuint vbo{};
  GLuint vao{};

public:
  p6::Context ctx = p6::Context{{1280, 720, "Amazing Oradimi Parzi_Val Boids"}};
  ;
  std::vector<GLuint> textureIDList;

  TrackballCamera camera;

  Renderer();

  // Texture
  void defineTextures();

  void addTexture(std::string path);

  // VBOs
  void defineVBO();

  // VAOs
  void defineVAO();

  void clearAll();

  void handleLookAround();

  void handleZoom();

  void drawEarth();

  void drawMoon();

  // Should be done last. It starts the infinite loop.
  void start() { ctx.start(); };

  void close() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  };
};
