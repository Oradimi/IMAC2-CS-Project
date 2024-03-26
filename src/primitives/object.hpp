#pragma once

#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include "texture.hpp"
#include <glm/glm.hpp>
#include <vector>

/// A class that stores everything needed to render an RenderedObject
class RenderedObject {
private:
  std::vector<glimac::ShapeVertex> mesh;
  std::vector<Texture> textureList;
  GLuint vbo{};
  GLuint vao{};

public:
  p6::Shader shader;
  GLint uMVPMatrix;
  GLint uMVMatrix;
  GLint uNormalMatrix;
  GLint uTexture;

  explicit RenderedObject(std::vector<glimac::ShapeVertex> mesh);

  void addTexture(std::string path);

  void defineTextures();

  void defineVBO();

  void defineVAO();

  std::vector<glimac::ShapeVertex> getMesh() const { return mesh; };

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