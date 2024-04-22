#include "objLoader.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<ShapeVertex> loadOBJ(const std::string &filename) {
  std::ifstream file("assets/models/" + filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << '\n';
    return {};
  }

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texCoords;

  std::vector<ShapeVertex> vertices{};

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

      int index = 0;
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

      addVertices(vertices, positions, normals, texCoords, vIndices, vtIndices,
                  vnIndices);
    }
  }
  file.close();

  return vertices;
}

void addVertices(std::vector<ShapeVertex> &vertices,
                 const std::vector<glm::vec3> &positions,
                 const std::vector<glm::vec3> &normals,
                 const std::vector<glm::vec2> &texCoords,
                 const std::vector<int> &vIndices,
                 const std::vector<int> &vtIndices,
                 const std::vector<int> &vnIndices) {
  if (vIndices.size() >= 3) {
    for (size_t i = 1; i < vIndices.size() - 1; ++i) {
      glm::vec3 pos0 = positions[vIndices[0]];
      glm::vec3 pos1 = positions[vIndices[i]];
      glm::vec3 pos2 = positions[vIndices[i + 1]];

      glm::vec3 normal0(0.f);
      glm::vec3 normal1(0.f);
      glm::vec3 normal2(0.f);
      glm::vec2 texCoords0(0.f);
      glm::vec2 texCoords1(0.f);
      glm::vec2 texCoords2(0.f);

      if (!vnIndices.empty() && i < vnIndices.size()) {
        normal0 = normals[vnIndices[0]];
        normal1 = normals[vnIndices[i]];
        normal2 = normals[vnIndices[i + 1]];
      }

      if (!vtIndices.empty() && i < vtIndices.size()) {
        texCoords0 = texCoords[vtIndices[0]];
        texCoords1 = texCoords[vtIndices[i]];
        texCoords2 = texCoords[vtIndices[i + 1]];
      }

      vertices.emplace_back(ShapeVertex{pos0, normal0, texCoords0});
      vertices.emplace_back(ShapeVertex{pos1, normal1, texCoords1});
      vertices.emplace_back(ShapeVertex{pos2, normal2, texCoords2});
    }
  }
}
