#include "cube_vertices.hpp"
#include <vector>

namespace glimac {

std::vector<ShapeVertex> cube_vertices(float x, float y, float z) {
  // Définition des sommets du cube
  std::vector<ShapeVertex> vertices{
      // Face avant
      {{-x, -y, z}, {0.f, 0.f, 1.f}, {0.f, 0.f}},
      {{x, -y, z}, {0.f, 0.f, 1.f}, {1.f, 0.f}},
      {{x, y, z}, {0.f, 0.f, 1.f}, {1.f, 1.f}},
      {{-x, y, z}, {0.f, 0.f, 1.f}, {0.f, 1.f}},
      // Face arrière
      {{-x, -y, -z}, {0.f, 0.f, -1.f}, {1.f, 0.f}},
      {{x, -y, -z}, {0.f, 0.f, -1.f}, {0.f, 0.f}},
      {{x, y, -z}, {0.f, 0.f, -1.f}, {0.f, 1.f}},
      {{-x, y, -z}, {0.f, 0.f, -1.f}, {1.f, 1.f}},
      // Face gauche
      {{-x, -y, -z}, {-1.f, 0.f, 0.f}, {0.f, 0.f}},
      {{-x, y, -z}, {-1.f, 0.f, 0.f}, {1.f, 0.f}},
      {{-x, y, z}, {-1.f, 0.f, 0.f}, {1.f, 1.f}},
      {{-x, -y, z}, {-1.f, 0.f, 0.f}, {0.f, 1.f}},
      // Face droite
      {{x, -y, -z}, {1.f, 0.f, 0.f}, {1.f, 0.f}},
      {{x, y, -z}, {1.f, 0.f, 0.f}, {0.f, 0.f}},
      {{x, y, z}, {1.f, 0.f, 0.f}, {0.f, 1.f}},
      {{x, -y, z}, {1.f, 0.f, 0.f}, {1.f, 1.f}},
      // Face haut
      {{-x, y, -z}, {0.f, 1.f, 0.f}, {0.f, 0.f}},
      {{x, y, -z}, {0.f, 1.f, 0.f}, {1.f, 0.f}},
      {{x, y, z}, {0.f, 1.f, 0.f}, {1.f, 1.f}},
      {{-x, y, z}, {0.f, 1.f, 0.f}, {0.f, 1.f}},
      // Face bas
      {{-x, -y, -z}, {0.f, -1.f, 0.f}, {1.f, 0.f}},
      {{x, -y, -z}, {0.f, -1.f, 0.f}, {0.f, 0.f}},
      {{x, -y, z}, {0.f, -1.f, 0.f}, {0.f, 1.f}},
      {{-x, -y, z}, {0.f, -1.f, 0.f}, {1.f, 1.f}}};

  return vertices;
}

} // namespace glimac