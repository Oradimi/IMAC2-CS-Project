#pragma once

#include "glimac/math.hpp"
#include "glm/ext/quaternion_exponential.hpp"
#include "glm/gtx/transform.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>

class TrackballCamera {
private:
  float m_Distance;
  float m_AngleX;
  float m_AngleY;

public:
  TrackballCamera() : m_Distance(173.2f), m_AngleX(0.f), m_AngleY(0.f){};
  TrackballCamera(const float distance, const float angleX, const float angleY)
      : m_Distance(distance), m_AngleX(angleX), m_AngleY(angleY){};

  void moveFront(const float &delta) {
    constexpr float min_val = 0.4f;
    constexpr float max_val = 293.2f;
    constexpr auto max_val_sqrt = ConstexprMath::sqrt<float>(max_val);
    m_Distance = std::clamp(m_Distance - delta * m_Distance / max_val_sqrt,
                            min_val, max_val);
  }

  void rotateLeft(const float &degrees) {
    m_AngleY += degrees;
    // std::cout << m_AngleY << " " << m_AngleX << std::endl;
  }

  void rotateUp(float degrees) {
    m_AngleX += degrees;
    // std::cout << m_AngleY << " " << m_AngleX << std::endl;
  }

  glm::mat4 getViewMatrix() const {
    glm::mat4 ViewMatrix = glm::mat4(1.f);
    ViewMatrix = glm::translate(ViewMatrix, {0.f, 0.f, -m_Distance});
    ViewMatrix =
        glm::rotate(ViewMatrix, glm::radians(m_AngleX), {1.f, 0.f, 0.f});
    ViewMatrix =
        glm::rotate(ViewMatrix, glm::radians(m_AngleY), {0.f, 1.f, 0.f});
    return ViewMatrix;
  }

  float getDistance() { return m_Distance; }

  // glm::mat4 getViewMatrixInverse() const
  // {
  //     glm::mat4 MVMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f,
  //     -m_Distance}); return MVMatrix * glm::rotate(MVMatrix,
  //     glm::radians(m_AngleX), {1.f, 0.f, 0.f}) * glm::rotate(MVMatrix,
  //     glm::radians(m_AngleY), {0.f, 1.f, 0.f});
  // }
};