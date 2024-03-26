#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <vector>

void loadOBJ(const char *filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }

  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texCoords;
  std::vector<std::vector<int>> faces;

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
      std::vector<int> face;
      int vIndex, tIndex, nIndex;
      //   char c;
      //   for (int i = 0; i < 3; ++i) {
      //     iss >> face.vertexIndex[i] >> c >> face.texCoordIndex[i] >> c >>
      //         face.normalIndex[i];
      //     // Decrement indices by 1 to match 0-based indexing
      //     --face.vertexIndex[i];
      //     --face.texCoordIndex[i];
      //     --face.normalIndex[i];
      //   }
      faces.push_back(face);
    }
  }
  file.close();
}
