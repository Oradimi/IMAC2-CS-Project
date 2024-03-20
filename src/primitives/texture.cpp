#include "texture.hpp"
#include "p6/p6.h"
#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glimac/cone_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"

Texture::Texture(std::string path)
  :image(p6::load_image_buffer("assets/textures/" + path)) {
    glGenTextures(1, &imagePointer);
}
  