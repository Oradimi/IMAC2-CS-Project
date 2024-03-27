#pragma once
#include "common.hpp"
#include <vector>

namespace glimac {

// Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère
// local) Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0)
// et (0, 0, 1)
std::vector<ShapeVertex> cube_vertices(float x, float y, float z);

} // namespace glimac
