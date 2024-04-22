#include "object.hpp"
#include <utility>

RenderedObject::RenderedObject(std::vector<ShapeVertex> mesh,
                               std::string texturePath,
                               std::string vertexShaderPath,
                               std::string fragmentShaderPath)
    : mesh(std::move(mesh)),
      shader{p6::load_shader("assets/shaders/" + vertexShaderPath,
                             "assets/shaders/" + fragmentShaderPath)},
      uMVPMatrix(glGetUniformLocation(shader.id(), "uMVPMatrix")),
      uMVMatrix(glGetUniformLocation(shader.id(), "uMVMatrix")),
      uNormalMatrix(glGetUniformLocation(shader.id(), "uNormalMatrix")),
      uTexture(glGetUniformLocation(shader.id(), "uTexture")),
      uKd(glGetUniformLocation(shader.id(), "uKd")),
      uKs(glGetUniformLocation(shader.id(), "uKs")),
      uShininess(glGetUniformLocation(shader.id(), "uShininess")),
      uWorldLightIntensity(
          glGetUniformLocation(shader.id(), "uWorldLightIntensity")),
      uLightIntensity(glGetUniformLocation(shader.id(), "uLightIntensity")),
      uLightDir_vs(glGetUniformLocation(shader.id(), "uLightDir_vs")),
      uLightPos_vs(glGetUniformLocation(shader.id(), "uLightPos_vs")),
      uLightCount(glGetUniformLocation(shader.id(), "uLightCount")) {
  addTexture(std::move(texturePath));

  defineVBO();
  defineVAO();

  defineTextures();
};

void RenderedObject::defineTextures() {
  for (int i = 0; i < getTextureCount(); i++) {
    textureList[i].getImage();
    glBindTexture(GL_TEXTURE_2D, textureList[i].getImagePointer());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureList[i].getImage().width(),
                 textureList[i].getImage().height(), 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, textureList[i].getImage().data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void RenderedObject::addTexture(std::string path) {
  Texture newTexture(path);
  textureList.emplace_back(std::move(newTexture));
}

void RenderedObject::defineVBO() {
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(ShapeVertex), mesh.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderedObject::defineVAO() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  static constexpr GLuint vertex_attr_position = 0;
  static constexpr GLuint vertex_attr_normal = 1;
  static constexpr GLuint vertex_attr_texcoords = 2;
  glEnableVertexAttribArray(vertex_attr_position);
  glEnableVertexAttribArray(vertex_attr_normal);
  glEnableVertexAttribArray(vertex_attr_texcoords);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE,
                        sizeof(ShapeVertex),
                        (const GLvoid *)offsetof(ShapeVertex, position));
  glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE,
                        sizeof(ShapeVertex),
                        (const GLvoid *)offsetof(ShapeVertex, normal));
  glVertexAttribPointer(vertex_attr_texcoords, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ShapeVertex),
                        (const GLvoid *)offsetof(ShapeVertex, texCoords));
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void RenderedObject::updateMesh(RandomMath &rand, p6::Context &ctx) {
  float waveAmplitude = 0.5f;
  float waveFrequency = 1.0f;
  for (int i = 0; i < mesh.size(); ++i) {
    glm::vec3 &vertex = mesh[i].position;
    float x = vertex.x;
    float z = vertex.z;

    float displacement = waveAmplitude * rand.generateIrwinHall();
    float waveOffset =
        glm::sin(ctx.delta_time() * waveFrequency + x + z) * displacement;
    vertex.y = waveOffset;
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(ShapeVertex), mesh.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
