#pragma once

#include "boid.hpp"
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "math.hpp"
#include "p6/p6.h"
#include "primitives/transform.hpp"
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

struct ViewAngle {
  float x;
  float y;
};

enum CameraMode { FREEFLY, TRACKBALL };

class Camera {
private:
  // Trackball mode
  float m_Distance;
  ViewAngle m_Angle;

  // Freefly mode
  // Transform m_WandererTransform;
  glm::vec3 m_Position;
  float m_Phi;   // in radians
  float m_Theta; // in radians
  glm::vec3 m_FrontVector;
  glm::vec3 m_LeftVector;
  glm::vec3 m_UpVector;
  float m_Speed = 30.f;

  void computeDirectionVectors() {
    m_FrontVector = {std::cosf(m_Theta) * std::sinf(m_Phi), std::sinf(m_Theta),
                     std::cosf(m_Theta) * std::cosf(m_Phi)};
    m_LeftVector = {std::sinf(m_Phi + p6::PI / 2.f), 0.f,
                    std::cosf(m_Phi + p6::PI / 2.f)};
    m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
  }

public:
  CameraMode cameraMode = TRACKBALL;

  Camera()
      : m_Distance(173.2f), m_Angle(0.f), m_Position(0.f, 12.f, 0.f),
        m_Phi(p6::PI), m_Theta(0.f) {
    computeDirectionVectors();
  };
  Camera(const float distance, const ViewAngle angle)
      : m_Distance(distance), m_Angle(angle) {
    computeDirectionVectors();
  };

  // Freefly function
  void moveLeft(const float &t) {
    m_Position += t * m_LeftVector;
    computeDirectionVectors();
  };

  void moveFront(const float &t) {
    switch (cameraMode) {
    case FREEFLY:
      m_Position += t * m_FrontVector;
      computeDirectionVectors();
      break;
    case TRACKBALL:
      constexpr float min_val = 0.4f;
      constexpr float max_val = 293.2f;
      constexpr auto max_val_sqrt = ConstexprMath::sqrt<float>(max_val);
      m_Distance = std::clamp(m_Distance - t * m_Distance / max_val_sqrt,
                              min_val, max_val);
      break;
    }
  }

  // Freefly function
  void moveUp(const float &t) {
    m_Position += t * m_UpVector;
    computeDirectionVectors();
  };

  void rotateLeft(const float &degrees) {
    switch (cameraMode) {
    case FREEFLY:
      m_Phi -= glm::radians(degrees);
      computeDirectionVectors();
      break;
    case TRACKBALL:
      m_Angle.y += degrees;
      break;
    }
  }

  void rotateUp(const float &degrees) {
    switch (cameraMode) {
    case FREEFLY:
      m_Theta -= glm::radians(degrees);
      m_Theta = glm::clamp(m_Theta, -p6::PI / 2.f + 0.1f, p6::PI / 2.f - 0.1f);
      computeDirectionVectors();
      break;
    case TRACKBALL:
      m_Angle.x += degrees;
      break;
    }
  }

  glm::mat4 getViewMatrix() const {
    switch (cameraMode) {
    case FREEFLY:
      return glm::lookAt(m_Position, m_Position + m_FrontVector,
                         glm::vec3(0.0f, 1.0f, 0.0f));
      break;
    case TRACKBALL:
      glm::mat4 ViewMatrix = glm::mat4(1.f);
      ViewMatrix = glm::translate(ViewMatrix, {0.f, 0.f, -m_Distance});
      ViewMatrix =
          glm::rotate(ViewMatrix, glm::radians(m_Angle.x), {1.f, 0.f, 0.f});
      ViewMatrix =
          glm::rotate(ViewMatrix, glm::radians(m_Angle.y), {0.f, 1.f, 0.f});
      ViewMatrix = glm::translate(ViewMatrix, {0.f, -Boid::getBounds(), 0.f});
      return ViewMatrix;
      break;
    }
  }

  // Trackball function
  float getDistance() const { return m_Distance; }

  // Freefly function
  float getSpeed() const { return m_Speed; }

  // Freefly function
  Transform getWandererTransform() const {
    return Transform{m_Position + 30.f * m_FrontVector + -15.f * m_UpVector,
                     {glm::degrees(-m_Theta), glm::degrees(m_Phi), 0.f},
                     3.f};
  }

  void switchCamera() {
    switch (cameraMode) {
    case FREEFLY:
      cameraMode = TRACKBALL;
      break;
    case TRACKBALL:
      cameraMode = FREEFLY;
      break;
    }
  }
};