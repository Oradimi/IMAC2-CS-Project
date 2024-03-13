#define DOCTEST_CONFIG_IMPLEMENT
#include "glimac/FreeflyCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

const std::vector<glimac::ShapeVertex> sphere =
    glimac::sphere_vertices(1.f, 32, 16);

struct EarthProgram {
  p6::Shader _Program;

  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;
  GLint uEarthTexture;
  GLint uCloudTexture;
  GLint uKd;
  GLint uKs;
  GLint uShininess;
  GLint uLightPos_vs;
  GLint uLightIntensity;
  GLint uTime;

  EarthProgram()
      : _Program{p6::load_shader("shaders/3D.vs.glsl",
                                 "shaders/pointLight.fs.glsl")} {
    uMVPMatrix = glGetUniformLocation(_Program.id(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(_Program.id(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(_Program.id(), "uNormalMatrix");
    uKd = glGetUniformLocation(_Program.id(), "uKd");
    uKs = glGetUniformLocation(_Program.id(), "uKs");
    uShininess = glGetUniformLocation(_Program.id(), "uShininess");
    uLightPos_vs = glGetUniformLocation(_Program.id(), "uLightPos_vs");
    uLightIntensity = glGetUniformLocation(_Program.id(), "uLightIntensity");
    uEarthTexture = glGetUniformLocation(_Program.id(), "uEarthTexture");
    uCloudTexture = glGetUniformLocation(_Program.id(), "uCloudTexture");
    uTime = glGetUniformLocation(_Program.id(), "uTime");
  }
};

struct MoonProgram {
  p6::Shader _Program;

  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;
  GLint uTexture;
  GLint uKd;
  GLint uKs;
  GLint uShininess;
  GLint uLightPos_vs;
  GLint uLightIntensity;

  MoonProgram()
      : _Program{p6::load_shader("shaders/3D.vs.glsl",
                                 "shaders/pointLight.fs.glsl")} {
    uMVPMatrix = glGetUniformLocation(_Program.id(), "uMVPMatrix");
    uMVMatrix = glGetUniformLocation(_Program.id(), "uMVMatrix");
    uNormalMatrix = glGetUniformLocation(_Program.id(), "uNormalMatrix");
    uKd = glGetUniformLocation(_Program.id(), "uKd");
    uKs = glGetUniformLocation(_Program.id(), "uKs");
    uShininess = glGetUniformLocation(_Program.id(), "uShininess");
    uLightPos_vs = glGetUniformLocation(_Program.id(), "uLightPos_vs");
    uLightIntensity = glGetUniformLocation(_Program.id(), "uLightIntensity");
    uTexture = glGetUniformLocation(_Program.id(), "uTexture");
  }
};

int main() {
  auto ctx = p6::Context{{1280, 720, "TP8 EX1"}};
  ctx.maximize_window();

  EarthProgram earthProgram{};
  MoonProgram moonProgram{};

  const img::Image imageEarth =
      p6::load_image_buffer("assets/textures/EarthMap.jpg");
  const img::Image imageMoon =
      p6::load_image_buffer("assets/textures/MoonMap.jpg");
  const img::Image imageCloud =
      p6::load_image_buffer("assets/textures/CloudMap.jpg");

  // Texture
  std::vector<GLuint> textures(3);
  glGenTextures(3, textures.data());

  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageEarth.width(),
               imageEarth.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               imageEarth.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageCloud.width(),
               imageCloud.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               imageCloud.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, textures[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageMoon.width(), imageMoon.height(),
               0, GL_RGBA, GL_UNSIGNED_BYTE, imageMoon.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  // VBOs
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sphere.size() * sizeof(glimac::ShapeVertex),
               sphere.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // VAOs
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  static constexpr GLuint vertex_attr_position = 0;
  static constexpr GLuint vertex_attr_normal = 1;
  static constexpr GLuint vertex_attr_texcoords = 2;
  glEnableVertexAttribArray(vertex_attr_position);
  glEnableVertexAttribArray(vertex_attr_normal);
  glEnableVertexAttribArray(vertex_attr_texcoords);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(
      vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, position));
  glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE,
                        sizeof(glimac::ShapeVertex),
                        (const GLvoid *)offsetof(glimac::ShapeVertex, normal));
  glVertexAttribPointer(
      vertex_attr_texcoords, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex),
      (const GLvoid *)offsetof(glimac::ShapeVertex, texCoords));
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  glEnable(GL_DEPTH_TEST);

  std::vector<glm::vec3> randomAxes{glm::sphericalRand(2.f)};
  for (int i = 1; i < 32; i++) {
    randomAxes.emplace_back(glm::sphericalRand(2.f));
  }

  FreeflyCamera camera;

  // Declare your infinite update loop.
  ctx.update = [&]() {
    ctx.background({});

    glm::vec3 lightPos{0.f, 0.f, 0.f};

    lightPos = glm::vec4(lightPos, 1.f) *
               glm::rotate(glm::mat4(1.f), ctx.time(), {0.f, 1.f, 0.f});
    lightPos += glm::vec3(0.f, 0.f, -5.f);

    glm::mat4 ProjMatrix =
        glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

    glm::mat4 MVMatrix = camera.getViewMatrix() *
                         glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f});

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    // Earth
    earthProgram._Program.use();
    float uKd = 4.f;
    float uKs = 4.f;
    float uLightIntensity = 0.36f;
    float uShininess = 50.f;
    glUniform1i(earthProgram.uEarthTexture, 0);
    glUniform1i(earthProgram.uCloudTexture, 1);
    glUniform3f(earthProgram.uKd, uKd, uKd, uKd);
    glUniform3f(earthProgram.uKs, uKs, uKs, uKs);
    glUniform3fv(
        earthProgram.uLightPos_vs, 1,
        glm::value_ptr(glm::vec4(lightPos, 1.f) * camera.getViewMatrix()));
    glUniform3f(earthProgram.uLightIntensity, uLightIntensity, uLightIntensity,
                uLightIntensity);
    glUniform1f(earthProgram.uShininess, uShininess);
    glUniform1f(earthProgram.uTime, ctx.time());

    glm::mat4 earthMVMatrix =
        glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});

    glUniformMatrix4fv(
        earthProgram.uNormalMatrix, 1, GL_FALSE,
        glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
    glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE,
                       glm::value_ptr(earthMVMatrix));
    glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE,
                       glm::value_ptr(ProjMatrix * earthMVMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glDrawArrays(GL_TRIANGLES, 0, sphere.size());

    // Moons
    moonProgram._Program.use();
    glUniform1i(moonProgram.uTexture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    for (int i = 0; i < 32; i++) {
      glm::mat4 moonMVMatrix = MVMatrix;
      moonMVMatrix = glm::rotate(moonMVMatrix, ctx.time(), {0.f, 1.f, 0.f});
      moonMVMatrix = glm::translate(moonMVMatrix, randomAxes[i]);
      moonMVMatrix = glm::scale(moonMVMatrix, glm::vec3{0.2f});

      glUniform3f(moonProgram.uKd, uKd, uKd, uKd);
      glUniform3f(moonProgram.uKs, uKs, uKs, uKs);
      glUniform3fv(
          moonProgram.uLightPos_vs, 1,
          glm::value_ptr(glm::vec4(lightPos, 1.f) * camera.getViewMatrix()));
      glUniform3f(moonProgram.uLightIntensity, uLightIntensity, uLightIntensity,
                  uLightIntensity);
      glUniform1f(moonProgram.uShininess, uShininess);

      glUniformMatrix4fv(
          moonProgram.uNormalMatrix, 1, GL_FALSE,
          glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
      glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE,
                         glm::value_ptr(moonMVMatrix));
      glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE,
                         glm::value_ptr(ProjMatrix * moonMVMatrix));

      glDrawArrays(GL_TRIANGLES, 0, sphere.size());
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    if (ctx.key_is_pressed(GLFW_KEY_W)) {
      camera.moveFront(ctx.delta_time() * 3.f);
    }

    if (ctx.key_is_pressed(GLFW_KEY_S)) {
      camera.moveFront(-ctx.delta_time() * 3.f);
    }

    if (ctx.key_is_pressed(GLFW_KEY_A)) {
      camera.moveLeft(ctx.delta_time() * 3.f);
    }

    if (ctx.key_is_pressed(GLFW_KEY_D)) {
      camera.moveLeft(-ctx.delta_time() * 3.f);
    }

    if (ctx.key_is_pressed(GLFW_KEY_SPACE)) {
      camera.moveUp(ctx.delta_time() * 3.f);
    }

    if (ctx.key_is_pressed(GLFW_KEY_LEFT_SHIFT)) {
      camera.moveUp(-ctx.delta_time() * 3.f);
    }
  };

  ctx.mouse_dragged = [&](p6::MouseDrag mouse_drag) {
    camera.rotateLeft(-mouse_drag.delta.x * 90.f);
    camera.rotateUp(mouse_drag.delta.y * 90.f);
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}