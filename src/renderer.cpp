#include "renderer.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "p6/p6.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <vector>

float Renderer::uWorldLightIntensity = 0.36f;
glm::vec3 Renderer::lightDir{0.5f, 0.5f, 0.5f};

Renderer::Renderer() {
  ImPlot::CreateContext();
  // ctx.maximize_window();
  glEnable(GL_DEPTH_TEST);
  glCullFace(GL_BACK);
};

void Renderer::addObject(RenderedObject object) {
  objectList.emplace_back(std::move(object));
}

void Renderer::addLight(Light light) { lightList.emplace_back(light); }

void Renderer::renderUI() {
  ctx.imgui = [&]() {
    ImGui::Begin("Parameters");
    if (ImGui::Button("Boids", ImVec2(120.0f, 0.0f)))
      switchTabs = BOID;
    ImGui::SameLine(0.0, 2.0f);
    if (ImGui::Button("Lights", ImVec2(120.0f, 0.0f)))
      switchTabs = LIGHT;
    ImGui::SameLine(0.0, 2.0f);
    if (ImGui::Button("Toggle Wanderer", ImVec2(120.0f, 0.0f)))
      camera.switchCamera();
    ImGui::SameLine(0.0, 2.0f);
    if (ImGui::Button("Math Debug", ImVec2(120.0f, 0.0f))) {
      math_test.initiate();
      math_debug_mode = true;
    }

    switch (switchTabs) {
    case BOID:
      Boid::initializeUIElements();
      break;
    case LIGHT:
      Renderer::initializeUIElements();
      break;
    case WANDERER:
      break;
    }
    ImGui::End();

    if (math_debug_mode)
      math_debug_mode = math_test.displayTestsGUI();
  };
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

void Renderer::handleKeyboard() {
  float speed = camera.getSpeed();

  if (ctx.key_is_pressed(GLFW_KEY_LEFT_SHIFT)) {
    speed *= 3.f;
  }

  if (ctx.key_is_pressed(GLFW_KEY_W)) {
    camera.moveFront(ctx.delta_time() * speed);
  }

  if (ctx.key_is_pressed(GLFW_KEY_S)) {
    camera.moveFront(-ctx.delta_time() * speed);
  }

  if (ctx.key_is_pressed(GLFW_KEY_A)) {
    camera.moveLeft(ctx.delta_time() * speed);
  }

  if (ctx.key_is_pressed(GLFW_KEY_D)) {
    camera.moveLeft(-ctx.delta_time() * speed);
  }

  if (ctx.key_is_pressed(GLFW_KEY_SPACE)) {
    camera.moveUp(ctx.delta_time() * speed);
  }
}

void Renderer::handleInputs() {
  switch (camera.cameraMode) {
  case FREEFLY:
    handleKeyboard();
    break;
  case TRACKBALL:
    handleZoom();
    break;
  }
  handleLookAround();
}

void Renderer::drawObject(const glm::mat4 &modelMatrix,
                          const RenderedObject &object) const {
  glm::mat4 viewMatrix = camera.getViewMatrix();
  glm::mat4 projMatrix =
      glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 1000.f);

  glBindVertexArray(object.getVAO());
  object.shader.use();

  glUniform3f(object.uKd, object.Kd, object.Kd, object.Kd);
  glUniform3f(object.uKs, object.Ks, object.Ks, object.Ks);
  glUniform3fv(
      object.uLightDir_vs, 1,
      glm::value_ptr(glm::vec4(lightDir, 1.f) * glm::inverse(viewMatrix)));
  glUniform3f(object.uWorldLightIntensity, uWorldLightIntensity,
              uWorldLightIntensity, uWorldLightIntensity);
  glUniform1f(object.uShininess, object.Shininess);

  int lightIndex = 0;
  glUniform1i(object.uLightCount, lightList.size());
  for (const Light &light : lightList) {
    glUniform3fv(object.uLightPos_vs + lightIndex, 1,
                 glm::value_ptr(viewMatrix * glm::vec4(light.position, 1.f)));
    glUniform3f(object.uLightIntensity + lightIndex, light.color.r,
                light.color.g, light.color.b);
    lightIndex++;
  }

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

void Renderer::initializeUIElements() {
  // ImGui::SliderFloat("Diffuse Reflection", &uKd, 0.f, 10.f);
  // ImGui::SliderFloat("Glossy Reflection", &uKs, 0.f, 10.f);
  ImGui::SliderFloat("Light Intensity", &uWorldLightIntensity, 0.f, 2.f);
  // ImGui::SliderFloat("Shininess", &uShininess, 0.f, 100.f);
  ImGui::SliderFloat("Light Direction X", &lightDir.x, -1.f, 1.f);
  ImGui::SliderFloat("Light Direction Y", &lightDir.y, -1.f, 1.f);
  ImGui::SliderFloat("Light Direction Z", &lightDir.z, -1.f, 1.f);
};
