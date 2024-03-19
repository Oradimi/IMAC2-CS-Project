#include "renderer.hpp"
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

const std::vector<glimac::ShapeVertex> sphere =
    glimac::sphere_vertices(-1.f, 32, 16);

std::vector<glm::vec3> createRandomlySpreadSpheres() {
  std::vector<glm::vec3> randomAxes{glm::sphericalRand(2.f)};
  for (int i = 1; i < 32; i++) {
    randomAxes.emplace_back(glm::sphericalRand(2.f));
  }
  return randomAxes;
};

Texture::Texture(std::string path)
  :image(p6::load_image_buffer("assets/textures/" + path)){}

Renderer::Renderer() {
  ctx.maximize_window();

  defineVBO();
  defineVAO();

  glEnable(GL_DEPTH_TEST);
};

// Texture
void Renderer::defineTextures() {
  textureIDList.insert(textureIDList.end(), textureList.size(), 0);
  glGenTextures(textureList.size(), textureIDList.data());
  for (int i = 0; i < textureList.size(); i++) {
    textureList[i].getImage();
    glBindTexture(GL_TEXTURE_2D, textureIDList[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureList[i].getImage().width(), textureList[i].getImage().height(), 0,
                GL_RGBA, GL_UNSIGNED_BYTE, textureList[i].getImage().data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void Renderer::addTexture(std::string path) {
  Texture newTexture(path);
  textureList.emplace_back(std::move(newTexture));
}

// VBOs
void Renderer::defineVBO() {
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sphere.size() * sizeof(glimac::ShapeVertex),
               sphere.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// VAOs
void Renderer::defineVAO() {
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
}

void Renderer::clearAll() {
  for (long i = 33984; i < 33992; i++) {
    glActiveTexture(i);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  glBindVertexArray(0);
  ctx.background({});
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::handleLookAround() {
  ctx.mouse_dragged = [&](p6::MouseDrag mouse_drag) {
    camera.rotateLeft(-mouse_drag.delta.x * 90.f);
    camera.rotateUp(mouse_drag.delta.y * 90.f);
  };
}

void Renderer::handleZoom() {
  ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
    if (scroll.dy > 0) {
      camera.moveFront(0.48f);
    } else if (scroll.dy < 0) {
      camera.moveFront(-0.48f);
    }
  };
};

void Renderer::drawEarth() {
  glm::mat4 ProjMatrix =
      glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
  glm::mat4 MVMatrix = camera.getViewMatrix();

  EarthProgram earthProgram{};
  glBindVertexArray(vao);
  earthProgram._Program.use();
  glUniform1i(earthProgram.uEarthTexture, 0);
  glUniform1i(earthProgram.uCloudTexture, 1);

  glm::mat4 earthMVMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -1.f}) *
                            glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f});

  glUniformMatrix4fv(
      earthProgram.uNormalMatrix, 1, GL_FALSE,
      glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
  glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE,
                     glm::value_ptr(earthMVMatrix));
  glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE,
                     glm::value_ptr(ProjMatrix * earthMVMatrix));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureIDList[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textureIDList[1]);
  glDrawArrays(GL_TRIANGLES, 0, sphere.size());
};

void Renderer::drawMoon(std::vector<glm::vec3> randomAxes) {
  glm::mat4 ProjMatrix =
      glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
  glm::mat4 MVMatrix = camera.getViewMatrix();

  MoonProgram moonProgram{};
  moonProgram._Program.use();
  glUniform1i(moonProgram.uTexture, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureIDList[2]);
  for (int i = 0; i < 32; i++) {
    glm::mat4 moonMVMatrix =
        glm::translate(glm::mat4{1.f}, {0.f, 0.f, -1.f}) * MVMatrix;
    moonMVMatrix = glm::rotate(moonMVMatrix, ctx.time(), {0.f, 1.f, 0.f});
    moonMVMatrix = glm::translate(moonMVMatrix, randomAxes[i]);
    moonMVMatrix = glm::scale(moonMVMatrix, glm::vec3{0.2f});

    glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE,
                       glm::value_ptr(moonMVMatrix));
    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE,
                       glm::value_ptr(ProjMatrix * moonMVMatrix));

    glDrawArrays(GL_TRIANGLES, 0, sphere.size());
  }
}
