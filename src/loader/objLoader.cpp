#include "glimac/common.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<glimac::ShapeVertex> loadOBJ(const std::string &filename) {
  std::ifstream file("assets/models/" + filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << '\n';
    return {};
  }

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texCoords;

  std::vector<glimac::ShapeVertex> vertices{};

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string type;
    iss >> type;

    if (type == "v") {
      glm::vec3 position;
      iss >> position.x >> position.y >> position.z;
      positions.push_back(position);
    } else if (type == "vn") {
      glm::vec3 normal;
      iss >> normal.x >> normal.y >> normal.z;
      normals.push_back(normal);
    } else if (type == "vt") {
      glm::vec2 texCoord;
      iss >> texCoord.s >> texCoord.t;
      texCoords.push_back(texCoord);
    } else if (type == "f") {
      int vIndex = 0;
      int vtIndex = 0;
      int vnIndex = 0;
      char c = 0;
      for (int i = 0; i < 3; ++i) {
        iss >> vIndex >> c >> vtIndex >> c >> vnIndex;

        // Decrement indices by 1 to match 0-based indexing
        --vIndex;
        --vtIndex;
        --vnIndex;

        vertices.emplace_back(glimac::ShapeVertex{
            {positions[vIndex]}, {normals[vnIndex]}, {texCoords[vtIndex]}});
      }
    }
  }
  file.close();

  return vertices;
}
