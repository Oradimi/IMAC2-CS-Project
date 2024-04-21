#pragma once

#include "boid.hpp"
#include "glm/gtx/transform.hpp"
#include "math.hpp"
#include <algorithm>
#include <glm/glm.hpp>

struct ViewAngle {
  float x;
  float y;
};

class TrackballCamera {
private:
  float m_Distance;
  ViewAngle m_Angle;

public:
  TrackballCamera() : m_Distance(173.2f), m_Angle(0.f){};
  TrackballCamera(const float distance, const ViewAngle angle)
      : m_Distance(distance), m_Angle(angle){};

  void moveFront(const float &delta) {
    constexpr float min_val = 0.4f;
    constexpr float max_val = 293.2f;
    constexpr auto max_val_sqrt = ConstexprMath::sqrt<float>(max_val);
    m_Distance = std::clamp(m_Distance - delta * m_Distance / max_val_sqrt,
                            min_val, max_val);
  }

  void rotateLeft(const float &degrees) {
    m_Angle.y += degrees;
    // std::cout << m_AngleY << " " << m_AngleX << std::endl;
  }

  void rotateUp(float degrees) {
    m_Angle.x += degrees;
    // std::cout << m_AngleY << " " << m_AngleX << std::endl;
  }

  glm::mat4 getViewMatrix() const {
    glm::mat4 ViewMatrix = glm::mat4(1.f);
    ViewMatrix = glm::translate(ViewMatrix, {0.f, 0.f, -m_Distance});
    ViewMatrix =
        glm::rotate(ViewMatrix, glm::radians(m_Angle.x), {1.f, 0.f, 0.f});
    ViewMatrix =
        glm::rotate(ViewMatrix, glm::radians(m_Angle.y), {0.f, 1.f, 0.f});
    ViewMatrix = glm::translate(ViewMatrix, {0.f, -Boid::getBounds(), 0.f});
    return ViewMatrix;
  }

  float getDistance() const { return m_Distance; }
};