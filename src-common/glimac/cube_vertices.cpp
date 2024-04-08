#include "cube_vertices.hpp"
#include <vector>

namespace glimac {

std::vector<ShapeVertex> cube_vertices(float x, float y, float z) {
  std::vector<ShapeVertex> vertices{
      // Front face
      {{-x, -y, z}, {0.f, 0.f, 1.f}, {0.f, 0.f}},
      {{x, -y, z}, {0.f, 0.f, 1.f}, {1.f, 0.f}},
      {{x, y, z}, {0.f, 0.f, 1.f}, {1.f, 1.f}},
      {{-x, y, z}, {0.f, 0.f, 1.f}, {0.f, 1.f}},
      // Back face
      {{-x, -y, -z}, {0.f, 0.f, -1.f}, {1.f, 0.f}},
      {{x, -y, -z}, {0.f, 0.f, -1.f}, {0.f, 0.f}},
      {{x, y, -z}, {0.f, 0.f, -1.f}, {0.f, 1.f}},
      {{-x, y, -z}, {0.f, 0.f, -1.f}, {1.f, 1.f}},
      // Left face
      {{-x, -y, -z}, {-1.f, 0.f, 0.f}, {0.f, 0.f}},
      {{-x, y, -z}, {-1.f, 0.f, 0.f}, {1.f, 0.f}},
      {{-x, y, z}, {-1.f, 0.f, 0.f}, {1.f, 1.f}},
      {{-x, -y, z}, {-1.f, 0.f, 0.f}, {0.f, 1.f}},
      // Right face
      {{x, -y, -z}, {1.f, 0.f, 0.f}, {1.f, 0.f}},
      {{x, y, -z}, {1.f, 0.f, 0.f}, {0.f, 0.f}},
      {{x, y, z}, {1.f, 0.f, 0.f}, {0.f, 1.f}},
      {{x, -y, z}, {1.f, 0.f, 0.f}, {1.f, 1.f}},
      // Upper face
      {{-x, y, -z}, {0.f, 1.f, 0.f}, {0.f, 0.f}},
      {{x, y, -z}, {0.f, 1.f, 0.f}, {1.f, 0.f}},
      {{x, y, z}, {0.f, 1.f, 0.f}, {1.f, 1.f}},
      {{-x, y, z}, {0.f, 1.f, 0.f}, {0.f, 1.f}},
      // Low face
      {{-x, -y, -z}, {0.f, -1.f, 0.f}, {1.f, 0.f}},
      {{x, -y, -z}, {0.f, -1.f, 0.f}, {0.f, 0.f}},
      {{x, -y, z}, {0.f, -1.f, 0.f}, {0.f, 1.f}},
      {{-x, -y, z}, {0.f, -1.f, 0.f}, {1.f, 1.f}}};

  return vertices;
}

} // namespace glimac