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
      std::vector<int> vIndices;
      std::vector<int> vtIndices;
      std::vector<int> vnIndices;

      int index;
      while (iss >> index) {
        vIndices.push_back(index - 1); // Convert to 0-based index
        if (iss.peek() == '/') {
          iss.ignore();
          if (iss.peek() != '/') {
            iss >> index;
            vtIndices.push_back(index - 1);
          }
          if (iss.peek() == '/') {
            iss.ignore();
            iss >> index;
            vnIndices.push_back(index - 1);
          }
        }
      }

      if (vIndices.size() >= 3) {
        for (size_t i = 1; i < vIndices.size() - 1; ++i) {
          vertices.emplace_back(glimac::ShapeVertex{positions[vIndices[0]],
                                                    normals[vnIndices[0]],
                                                    texCoords[vtIndices[0]]});
          vertices.emplace_back(glimac::ShapeVertex{positions[vIndices[i]],
                                                    normals[vnIndices[i]],
                                                    texCoords[vtIndices[i]]});
          vertices.emplace_back(glimac::ShapeVertex{
              positions[vIndices[i + 1]], normals[vnIndices[i + 1]],
              texCoords[vtIndices[i + 1]]});
        }
      }
    }
  }
  file.close();

  return vertices;
}
