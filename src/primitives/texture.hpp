#pragma once

#include <string>
#include "img/src/Image.h"

class Texture {
private:
  img::Image image;

public:
  explicit Texture(std::string path);
  const img::Image& getImage() const {
    return image;
  }
};
