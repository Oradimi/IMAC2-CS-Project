#include "renderer.hpp"
#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "p6/p6.h"
#include "programs.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

Renderer::Renderer() {
  ctx.maximize_window();
  glEnable(GL_DEPTH_TEST);
};

void Renderer::addObject(RenderedObject object) {
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

void Renderer::drawObject(/*RenderedObject &object*/) const {
  glm::mat4 ProjMatrix =
      glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
  glm::mat4 MVMatrix = camera.getViewMatrix();
  for (const RenderedObject &object : objectList) {
    glBindVertexArray(object.getVAO());
    object.shader.use();
    glUniform1i(object.uTexture, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object.getTexturePointer(0));

    glm::mat4 objectMVMatrix =
        glm::translate(glm::mat4{1.f}, {0.f, 0.f, -1.f}) * MVMatrix;
    objectMVMatrix = glm::rotate(objectMVMatrix, ctx.time(), {0.f, 1.f, 0.f});
    // objectMVMatrix = glm::translate(objectMVMatrix, object._randomAxes[i]);
    objectMVMatrix = glm::scale(objectMVMatrix, glm::vec3{0.2f});

    glUniformMatrix4fv(object.uMVMatrix, 1, GL_FALSE,
                       glm::value_ptr(objectMVMatrix));
    glUniformMatrix4fv(object.uMVPMatrix, 1, GL_FALSE,
                       glm::value_ptr(ProjMatrix * objectMVMatrix));

    glDrawArrays(GL_TRIANGLES, 0, object.getMesh().size());
  }
};
