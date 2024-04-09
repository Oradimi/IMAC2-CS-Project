#include "renderer.hpp"
#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/matrix.hpp"
#include "p6/p6.h"
#include "programs.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <vector>

float Renderer::uKd = 1.f;
float Renderer::uKs = 3.f;
float Renderer::uLightIntensity = 0.36f;
float Renderer::uShininess = 4.f;
glm::vec3 Renderer::lightDir{0.5f, 0.5f, 0.5f};

Renderer::Renderer() {
  ctx.maximize_window();
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
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
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
      return;
    camera.rotateLeft(-mouse_drag.delta.x * 90.f);
    camera.rotateUp(mouse_drag.delta.y * 90.f);
  };
}

void Renderer::handleZoom() {
  ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
    if (scroll.dy > 0) {
      camera.moveFront(4.8f);
    } else if (scroll.dy < 0) {
      camera.moveFront(-4.8f);
    }
  };
};

void Renderer::drawObject(glm::mat4 &modelMatrix,
                          RenderedObject &object) const {

  lightDir = glm::vec4(lightDir, 1.f) *
             glm::rotate(glm::mat4(1.f), 0.f, {0.f, 1.f, 0.f});

  glm::mat4 viewMatrix = camera.getViewMatrix();
  glm::mat4 projMatrix =
      glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 1000.f);

  glBindVertexArray(object.getVAO());
  object.shader.use();

  glUniform3f(object.uKd, uKd, uKd, uKd);
  glUniform3f(object.uKs, uKs, uKs, uKs);
  glUniform3fv(
      object.uLightDir_vs, 1,
      glm::value_ptr(glm::vec4(lightDir, 1.f) * camera.getViewMatrix()));
  glUniform3f(object.uLightIntensity, uLightIntensity, uLightIntensity,
              uLightIntensity);
  glUniform1f(object.uShininess, uShininess);
  glUniform1i(object.uTexture, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, object.getTexturePointer(0));

  glUniformMatrix4fv(
      object.uNormalMatrix, 1, GL_FALSE,
      glm::value_ptr(glm::transpose(glm::inverse(viewMatrix * modelMatrix))));
  glUniformMatrix4fv(object.uMVMatrix, 1, GL_FALSE,
                     glm::value_ptr(viewMatrix * modelMatrix));
  glUniformMatrix4fv(object.uMVPMatrix, 1, GL_FALSE,
                     glm::value_ptr(projMatrix * viewMatrix * modelMatrix));

  glDrawArrays(GL_TRIANGLES, 0, object.getMesh().size());
};

glm::mat4 computeRotationMatrix(const glm::vec3 &velocity) {
  glm::vec3 direction = glm::normalize(velocity);

  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::quat rotation = glm::rotation(up, direction);

  glm::mat4 rotationMatrix = glm::mat4_cast(rotation);

  return rotationMatrix;
};

void Renderer::initializeUIElements() {
  ImGui::SliderFloat("Diffuse Reflection", &uKd, 0.f, 10.f);
  ImGui::SliderFloat("Glossy Reflection", &uKs, 0.f, 10.f);
  ImGui::SliderFloat("Light Intensity", &uLightIntensity, 0.f, 2.f);
  ImGui::SliderFloat("Shininess", &uShininess, 0.f, 100.f);
  ImGui::SliderFloat("Light Direction X", &lightDir.x, -1.f, 1.f);
  ImGui::SliderFloat("Light Direction Y", &lightDir.y, -1.f, 1.f);
  ImGui::SliderFloat("Light Direction Z", &lightDir.z, -1.f, 1.f);
};
