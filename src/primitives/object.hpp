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

class Object {
private:
    std::vector<glimac::ShapeVertex> mesh;
    std::vector<Texture> textureList;
    GLuint vbo{};
    GLuint vao{};

public:
    explicit Object(std::vector<glimac::ShapeVertex> mesh);

    void addTexture(std::string path);

    void defineTextures();

    void defineVBO();

    void defineVAO();

    std::vector<glimac::ShapeVertex> getMesh() { return mesh; };

    GLuint getVAO() { return vao; };

    int getTextureCount() { return textureList.size(); };

    int getTexturePointer(int i) { return textureList[i].getImagePointer(); };

    void close() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    };
};
