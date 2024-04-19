#pragma once

#include "img/src/Image.h"
#include "p6/p6.h"
#include <string>

class Texture {
private:
  img::Image image;
  GLuint imagePointer;

public:
  explicit Texture(std::string &path);
  const img::Image &getImage() const { return image; };
  const GLuint &getImagePointer() const { return imagePointer; };
};
