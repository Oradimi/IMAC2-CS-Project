#include "texture.hpp"

Texture::Texture(std::string &path) // NOLINT
    : image(p6::load_image_buffer("assets/textures/" + path)) {
  // We do want an uninitialized variable to get a random unassigned number
  glGenTextures(1, &imagePointer);
}
