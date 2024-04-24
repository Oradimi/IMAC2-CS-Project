#pragma once

#include "math.hpp"
#include "p6/p6.h"
#include "shape_vertex.hpp"
#include "texture.hpp"
#include <glm/glm.hpp>
#include <vector>

struct HashPair {
  template <typename T1, typename T2>
  std::size_t operator()(const std::pair<T1, T2> &p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

/// A class that stores everything needed to render an RenderedObject
class RenderedObject {
private:
  std::vector<ShapeVertex> mesh;
  std::vector<Texture> textureList;
  GLuint vbo{};
  GLuint vao{};

public:
  p6::Shader shader;
  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;
  GLint uTexture;
  GLint uKd;
  GLint uKs;
  GLint uShininess;
  GLint uWorldLightIntensity;
  GLint uLightIntensity;
  GLint uLightDir_vs;
  GLint uLightPos_vs;
  GLint uLightCount;

  float Kd = 1.f;
  float Ks = 3.f;
  float Shininess = 4.f;

  explicit RenderedObject(std::vector<ShapeVertex> mesh,
                          std::string texturePath, std::string vertexShaderPath,
                          std::string fragmentShaderPath);

  void addTexture(std::string path);

  void defineTextures();

  void defineVBO();

  void defineVAO();

  void updateWave(RandomMath &rand, p6::Context &ctx,
                  std::unordered_map<std::pair<float, float>, float, HashPair>
                      &waveOffsets);

  std::vector<ShapeVertex> getMesh() const { return mesh; };

  GLuint getVAO() const { return vao; };

  int getTextureCount() const { return textureList.size(); };

  int getTexturePointer(int i) const {
    return textureList[i].getImagePointer();
  };

  void clear() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  };
};