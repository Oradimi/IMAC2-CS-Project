#include "renderer.hpp"
#include "programs.hpp"
#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glimac/cone_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "p6/p6.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

Renderer::Renderer() {
  ctx.maximize_window();
  glEnable(GL_DEPTH_TEST);
};

void Renderer::addObject(Object& object) {
  objectList.emplace_back(std::move(object));
}

void Renderer::clearAll() {
  for (int i = 0; i < 16; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
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

void Renderer::drawEarth(EarthProgram& earthProgram) const {
  glm::mat4 ProjMatrix =
      glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
  glm::mat4 MVMatrix = camera.getViewMatrix();
  glBindVertexArray(earthProgram._Object.getVAO());
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

  for (int i = 0; i < earthProgram._Object.getTextureCount(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, earthProgram._Object.getTexturePointer(i));
  }
  glDrawArrays(GL_TRIANGLES, 0, earthProgram._Object.getMesh().size());
};

void Renderer::drawMoon(MoonProgram& moonProgram) const {
  glm::mat4 ProjMatrix =
      glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
  glm::mat4 MVMatrix = camera.getViewMatrix();
  glBindVertexArray(moonProgram._Object.getVAO());
  moonProgram._Program.use();
  glUniform1i(moonProgram.uTexture, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, moonProgram._Object.getTexturePointer(0));
  for (int i = 0; i < 32; i++) {
    glm::mat4 moonMVMatrix =
        glm::translate(glm::mat4{1.f}, {0.f, 0.f, -1.f}) * MVMatrix;
    moonMVMatrix = glm::rotate(moonMVMatrix, ctx.time(), {0.f, 1.f, 0.f});
    moonMVMatrix = glm::translate(moonMVMatrix, moonProgram._randomAxes[i]);
    moonMVMatrix = glm::scale(moonMVMatrix, glm::vec3{0.2f});

    glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE,
                       glm::value_ptr(moonMVMatrix));
    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE,
                       glm::value_ptr(ProjMatrix * moonMVMatrix));

    glDrawArrays(GL_TRIANGLES, 0, moonProgram._Object.getMesh().size());
  }
}
