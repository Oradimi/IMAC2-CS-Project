#include "texture.hpp"
#include "p6/p6.h"

Texture::Texture(std::string path)
  :image(p6::load_image_buffer("assets/textures/" + path)){}
  