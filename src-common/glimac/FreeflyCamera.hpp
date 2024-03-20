#pragma once

#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "p6/p6.h"
#include <cmath>


class FreeflyCamera {
private:
  glm::vec3 m_Position;
  float m_Phi;
  float m_Theta;
  glm::vec3 m_FrontVector;
  glm::vec3 m_LeftVector;
  glm::vec3 m_UpVector;

  void computeDirectionVectors() {
    m_FrontVector = {std::cosf(m_Theta) * std::sinf(m_Phi), std::sinf(m_Theta),
                     std::cosf(m_Theta) * std::cosf(m_Phi)};
    m_LeftVector = {std::sinf(m_Phi + p6::PI / 2.f), 0.f,
                    std::cosf(m_Phi + p6::PI / 2.f)};
    m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
    // std::cout << m_FrontVector.x << " " << m_FrontVector.y << " " <<
    // m_FrontVector.z << " " << std::endl; std::cout << m_LeftVector.x << " "
    // << m_LeftVector.y << " " << m_LeftVector.z << " " << std::endl; std::cout
    // << m_UpVector.x << " " << m_UpVector.y << " " << m_UpVector.z << " " <<
    // std::endl;
  }

public:
  FreeflyCamera() : m_Position(0.f, 0.f, 0.f), m_Phi(p6::PI), m_Theta(0.f) {
    computeDirectionVectors();
  };

  void moveLeft(float t) {
    m_Position += t * m_LeftVector;
    computeDirectionVectors();
    // std::cout << m_Position.x << " " << m_Position.y << " " << m_Position.z
    // << " " << std::endl;
  };

  void moveFront(float t) {
    m_Position += t * m_FrontVector;
    computeDirectionVectors();
    // std::cout << m_Position.x << " " << m_Position.y << " " << m_Position.z
    // << " " << std::endl;
  };

  void moveUp(float t) {
    m_Position += t * m_UpVector;
    computeDirectionVectors();
    // std::cout << m_Position.x << " " << m_Position.y << " " << m_Position.z
    // << " " << std::endl;
  };

  void rotateLeft(float degrees) {
    m_Phi -= glm::radians(degrees);
    computeDirectionVectors();
  };

  void rotateUp(float degrees) {
    m_Theta -= glm::radians(degrees);
    m_Theta = glm::clamp(m_Theta, -p6::PI / 2.f + 0.1f, p6::PI / 2.f - 0.1f);
    computeDirectionVectors();
  };

  glm::mat4 getViewMatrix() const {
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
  };
};
